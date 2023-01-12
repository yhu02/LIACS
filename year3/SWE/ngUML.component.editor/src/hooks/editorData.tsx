import React from 'react'
import * as uuid from 'uuid'
import _ from 'lodash'
import cytoscape from 'cytoscape'
import configuration from '../config'

interface IProperty {
    name: string;
    type: string;
    id?: number;
}

interface IMethod {
    name: string;
    type: string;
    code: string;
    id?: number;
}

interface INode {
    name: string;
    type: string;
    position: {
        x: number, 
        y: number
    };
    data: Record<
        string,
        string | number | IProperty[] | IMethod[]
    >;
    instances: any;
    id?: number;
}

interface IConnection {
    name: string;
    label?: string | React.ReactElement;
    labelFrom?: string | React.ReactElement;
    labelTo?: string | React.ReactElement;
    type: string;
    from: string;
    to: string;
    id?: number;
}

interface INodes {
    [uuid: string] : INode
}

interface IConnections {
    [uuid: string] : IConnection
}

export interface IActivity{
    id: number;
    name: string;
    condition: {
        'pre-condition':string
        'post-condition':string
    };
    isReadOnly:boolean,
    isSingleExecution:boolean,
    call_behaviourAction_id?:number | null
}

interface IActivities{
    [uuid: string] : IActivity
}

interface IEditorData {
    nodes: INodes;
    setNodes: (value: INodes) => void;
    copyNode: (uuid: string) => void;
    deleteNode: (uuid: string) => void;
    addNode: (type: string) => void;
    addSubNode: (type: string) => void;
    moveNode: (uuid: string, x: number, y: number) => void;
    references: Record<string, React.RefObject<HTMLElement>>;
    addRef: (uuid: string, ref: React.RefObject<HTMLElement>) => void;
    connections: IConnections;
    setConnections: (value: IConnections) => void;
    activityName:string;
    setActivityName: (value:string) => void;
    activities:IActivities,
    setActivities:(data:any) => void;
    currentActivity:IActivity;
    setCurrentActivity:(data:any) => void;
    importedNodes: INodes;
    importedConnections: IConnections;
    importData: (id?:number,import_activity?:boolean) => void;
    getActivities: () => void;
    importDataLocal: (event: any) => void;
    writeData: (data: any, done: any, error: any) => void;
    writeDataLocal: (data: any, done: any, error: any) => void;
    autoLayout: () => void;
    layout: boolean;
    forceRefresh: number;
}

const EditorData = React.createContext<
    IEditorData | undefined
>(undefined)

export const EditorDataProvider: React.FC = ({children}: React.PropsWithChildren) => {
    // Example object that contains nodes and their positions
    const [
        nodes, 
        setNodes
    ] = React.useState<INodes>({})
    
    const [
        importedNodes, 
        setImportedNodes
    ] = React.useState<INodes>({})

    const [
        layout,
        setLayout
    ] = React.useState<boolean>(false)

    const [
        forceRefresh,
        setForceRefresh
    ] = React.useState<number>(new Date().getTime())

    const[
        activityName,
        setActivityName
    ] = React.useState<string>('');

    const[
        activities,
        setActivities
    ] = React.useState<IActivities>({});

    const[
        currentActivity,
        setCurrentActivity
    ] = React.useState<IActivity>({name:'',id:0, condition:{'post-condition':'','pre-condition':''},isReadOnly:false,isSingleExecution:false});

    const moveNode = (uuid: string, x: number, y: number) => {
        setNodes({
            ...nodes,
            [uuid]: {
                ...nodes[uuid],
                position: {
                    x: x,
                    y: y
                }
            }
        })
    }

    const addNode = (type: string) => {
        setNodes({
            ...nodes,
            [uuid.v4()]: {
                name: 'new'+type+'Node',
                type: type,
                position: {
                    x: 128,
                    y: 128
                },
                data: {},
                instances: {}
            }
        })
    }

    const addSubNode = (type: string) => {
        setNodes({
            ...nodes,
            [uuid.v4()]: {
                name: 'new'+type+'subNode',
                type: type,
                position: {
                    x: 256, //inside father component!
                    y: 256
                },
                data: {},
                instances: {}
            }
        })
    }

    const copyNode = (node: string) => {
        // Deep Copy
        let newNode = JSON.parse(JSON.stringify(nodes[node]))
        
        setNodes({
            ...nodes,
            [uuid.v4()]: {
                ...newNode,
                position: {
                    x: newNode['position']['x'] + 32,
                    y: newNode['position']['y'] + 32,
                }
            }
        })
    }

    const deleteNode = (node: string) => {
        let newObj = nodes
        delete newObj[node]
        let newRefs: any = references
        delete newRefs[node]
        let newConnections = connections
        Object.keys(connections).forEach((e) => {
            if (connections[e].from === node) {
                delete connections[e]
            } else if (connections[e].to === node) {
                delete connections[e]
            }
        })
        setNodes({...newObj})
        setReferences({...newRefs})
        setConnections({...newConnections})
    }

    const [references, setReferences] = React.useState({
        'root': React.useRef(document.getElementById('root'))
    })

    const addRef = (
        uuid: string, 
        ref: React.RefObject<HTMLElement>
    ) => { 
        setReferences(
            (refs) => {
                return {
                    ...refs,
                    [uuid]: ref
                }
            })
    }

    const [
        connections, 
        setConnections
    ] = React.useState<IConnections>({})
    
    const [
        importedConnections, 
        setImportedConnections
    ] = React.useState<IConnections>({})

    const importData = (id?:number,import_activity?:boolean) => {
        // GET NODES
        fetch(configuration.url+"model/data?uml-type="+(import_activity? 'activity' : 'class')+(id && id != 0 ?"&activity-id="+id  :''))
            .then(response => {console.log(response);return response.json()})
            .then(data => {
                if(configuration.environment == 'dev'){
                    console.log(data);
                }
                setImportedNodes(data.nodes ?? {})
                setNodes(
                    JSON.parse(
                        JSON.stringify(data.nodes)
                    ) ?? {}
                )
                setImportedConnections(data.connections ?? {})
                setConnections(
                    JSON.parse(
                        JSON.stringify(data.connections)
                    ) ?? {}
                )
            })
    }

    const getActivities = () => {
        fetch(configuration.url+"model/data?uml-type=activity&request-type=activities")
            .then(response => {
                response.json().then(data => {
                    setActivities(data.activities);
                    console.log(data);
                });
            }).catch(error => {
                console.log("Could not connect to the backend, is it running?");
                console.log(error);
            })
        }
    const importDataLocal = (data:any) => {
        let jsonData = JSON.parse(data);
        
        setImportedNodes(jsonData.nodes ?? {})
        setNodes(
            JSON.parse(
                JSON.stringify(jsonData.nodes)
            ) ?? {}
        )
        setImportedConnections(jsonData.connections ?? {})
        setConnections(
            JSON.parse(
                JSON.stringify(jsonData.connections)
            ) ?? {}
        )
    }

    const writeData = (data: any, done: any, error: any) => {
        if(configuration.environment == 'dev'){
            console.log(nodes,connections,data);
        }
        let activities = 0;
        let classes = 0;
        for(let i in nodes){
            if(nodes[i].type == 'Class' || nodes[i].type == 'Enum')
                classes++;
            else
                activities ++;
        }
        const type = classes > activities ? 'class' : 'activity';
        fetch(configuration.url+"model/data?uml-type="+type,
        {
            method: 'POST',
            mode: 'no-cors',
            body: JSON.stringify({
                nodes: nodes,
                connections: connections,
                changes: data
            })
        }).then((response)=>{
            const nodesCopy = JSON.parse(JSON.stringify(nodes));//make a copy of the info so it doesnt call by reference
            const connectionsCopy = JSON.parse(JSON.stringify(connections));

            setImportedNodes(nodesCopy);
            setImportedConnections(connectionsCopy);
        });
    }

    const writeDataLocal = (data: any, done: any, error: any) => {
        const a = document.createElement('a');

        let jsonStr = JSON.stringify({
            nodes: nodes,
            connections: connections,
            changes: data
        });
        
        const file = new Blob([jsonStr], {type: 'text/json'});
        a.href = URL.createObjectURL(file);
        a.download = "nguml-data.json";
        a.click();

        URL.revokeObjectURL(a.href);
    }

    const autoLayout = () => {
        setLayout(true)
        
        let gNodes = Object.keys(nodes).map(
            (key: string) => {
                return {
                    'group': 'nodes',
                    'data': {'id': key},
                }
            }
        )

        let gLinks = Object.keys(connections).map(
            (key: string) => {
                return {
                    group: 'edges',
                    data: {
                        id: key,
                        source: connections[key].from,
                        target: connections[key].to
                    }
                }
            }
        )
        
        let cy = cytoscape({
            headless: true
        })

        cy.add(gNodes as any)
        cy.add(gLinks as any)

        let layout = cy.layout({
            name: 'cose'
        })

        layout.on('layoutstop', () => {
            Object.keys(nodes).forEach(
                (key: string) => {
                    let pos = cy.$id(key).position()
                    setNodes({
                        ...nodes,
                        [key]: {
                            ...nodes[key],
                            position: {
                                x: pos.x * 6,
                                y: pos.y * 4
                            }
                        } 
                    })
                    setTimeout(
                        () => setForceRefresh(new Date().getTime()),
                        50
                        )
                    setLayout(false)
                }
            )
        })

        layout.run()
        
    }

    return (
        <EditorData.Provider
            value={{
                nodes,
                setNodes,
                copyNode,
                deleteNode,
                addNode,
                addSubNode,
                moveNode,
                references,
                addRef,
                connections,
                setConnections,
                activityName,
                setActivityName,
                activities,
                setActivities,
                getActivities,
                currentActivity,
                setCurrentActivity,
                importedNodes,
                importedConnections,
                importData,
                importDataLocal,
                writeData,
                writeDataLocal,
                layout,
                autoLayout,
                forceRefresh
            }}
        >
            {children}
        </EditorData.Provider>
    )
}

export default EditorData