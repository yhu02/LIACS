import React from 'react'
import EditorData from '../../hooks/editorData'
import { 
    HTMLTable,
    Icon,
    Button,
    Alert,
    ButtonGroup,
    EditableText,
} from '@blueprintjs/core'
import _ from 'lodash'
import * as uuid from "uuid";

const getDifferencesMethods = (
    node: any,
    importedMethods: any,
    methods: any
) => {
    let changes: any[] = []
    let dontChange: any[]

    _.differenceWith(
        methods, 
        importedMethods, 
        _.isEqual
    ).forEach(
        (value: any) => {
            if (value && value.id) {
                if (
                    importedMethods.filter(
                        (iValue: any) => {
                            return iValue.id === value.id
                        }
                    )[0].type !== value.type
                ) {
                    changes.push({
                        type: 'retype-method',
                        from: node,
                        to: value
                    })
                    dontChange.push(value.id)
                } else {
                    changes.push({
                        type: 'new-method',
                        from: node,
                        to: value
                    })
                }
            } else if (value) {
                changes.push({
                    type: 'new-method',
                    from: node,
                    to: value
                })
            }
        }
    )
    
    _.differenceWith(
        importedMethods, 
        methods, 
        _.isEqual
    ).forEach(
        (value: any) => {
            if (value && dontChange.indexOf(value.id)) {
                changes.push({
                    type: 'delete-method',
                    from: node,
                    to: value
                })
            }
        }
    )

    return changes
}

const getDifferencesProperties = (
    node: any,
    importedProperties: any,
    properties: any
) => {
    let changes: any[] = []
    let dontChange: any[] = []
    
    console.log(properties)
    console.log(importedProperties)

    _.differenceWith(
        properties, 
        importedProperties, 
        _.isEqual
    ).forEach(
        (value: any) => {
            if (value && value.id) {
                let sameProperties = importedProperties.filter(
                    (iValue: any) => {
                        return iValue.id === value.id
                    }
                )
                
                if (sameProperties[0].type !== value.type) {
                    changes.push({
                        type: 'retype-property',
                        from: node,
                        to: value
                    })
                    dontChange.push(value.id)
                } else {
                    changes.push({
                        type: 'new-property',
                        from: node,
                        to: value
                    })
                }
            } else if (value) {
                changes.push({
                    type: 'new-property',
                    from: node,
                    to: value
                })
            }
        }
    )
    
    _.differenceWith(
        importedProperties, 
        properties, 
        _.isEqual
    ).forEach(
        (value: any) => {
            if (value && dontChange.indexOf(value.id) < 0) {
                changes.push({
                    type: 'delete-property',
                    from: node,
                    to: value
                })
            }
        }
    )

    return changes
}

const getNewNodes = (
    importedNodes: any,
    nodes: any
) => {
    let changes = Object.keys(nodes).filter(
        (keyName: string) => {
            return !importedNodes[keyName]
        }
    ).map(
        (keyName: string) => {
            let newMap = []
            newMap.push({
                type: 'new-classifier',
                from: null,
                to: nodes[keyName],
                nodeKey: keyName
            })
            newMap = newMap.concat(
                getDifferencesProperties(
                    nodes[keyName],
                    [],
                    nodes[keyName]['data']['properties']
                )
            )
            newMap = newMap.concat(
                getDifferencesMethods(
                    nodes[keyName],
                    [],
                    nodes[keyName]['data']['methods']
                )
            )
            newMap = newMap.concat(
                getDifferencesMethods(
                    nodes[keyName],
                    [],
                    nodes[keyName]['data']['subcomponents']
                )
            )
            return newMap
        }
    )
    let newNodes: any[] = []
    changes.forEach(
        (value: any) => {
            if (value) {
                newNodes = newNodes.concat(value)
            }
        }
    )
    return newNodes.filter(
        (value: any) => {
            return value
        }
    )
}

const getModifiedNodes = (
    importedNodes: any,
    nodes: any
) => {
    let changes = Object.keys(nodes).filter(
        (keyName: string) => {
            return importedNodes[keyName]
        }
    ).map(
        (keyName: string) => {
            let node = [
                importedNodes[keyName],
                nodes[keyName]
            ]
            let newMap: any[] = []
            if(node[0]['data']['properties'])
                newMap = newMap.concat(
                    getDifferencesProperties(
                        nodes[keyName],
                        node[0]['data']['properties'], 
                        node[1]['data']['properties']
                    )
                )
            if(node[0]['data']['methods'])
                newMap = newMap.concat(
                    getDifferencesMethods(
                        nodes[keyName],
                        node[0]['data']['methods'], 
                        node[1]['data']['methods']
                    )
                )
            return newMap
        }
    )
    let modifiedNodes: any[] = []
    changes.forEach(
        (value: any) => {
            if (value) {
                modifiedNodes = modifiedNodes.concat(value)
            }
        }
    )
    return modifiedNodes.filter(
        (value: any) => {
            return value
        }
    )
}

const getDifferencesNodes = (
    importedNodes: any,
    nodes: any
) => {
    const newNodes = getNewNodes(importedNodes, nodes)
    const modifiedNodes = getModifiedNodes(importedNodes, nodes)
    return ([] as any[]).concat(newNodes, modifiedNodes)
}

const getNewConnections = (
    nodes: any,
    importedConnections: any,
    connections: any
) => {
    let changes = Object.keys(connections).filter(
        (keyName: string) => {
            return !importedConnections[keyName]
        }
    ).map(
        (keyName: string) => {
            let newMap = []
            newMap.push({
                type: 'new-connection',
                from: [
                    nodes[
                        connections[keyName]['from']
                    ]['name'],
                    nodes[
                        connections[keyName]['to']
                    ]['name']
                ],
                to: connections[keyName]
            })
            return newMap
        }
    )

    let newConnections: any[] = []
    changes.forEach(
        (value: any) => {
            if (value) {
                newConnections = newConnections.concat(value)
            }
        }
    )
    return newConnections.filter(
        (value: any) => {
            return value
        }
    )
}

const getModifiedConnections = (
    importedConnections: any,
    connections: any
) => {
    let dontChange: number[] = []
    let modifiedConnections = Object.keys(connections).filter(
        (keyName: string) => {
            return importedConnections[keyName]
        }
    ).filter(
        (keyName: string) => {
            return !_.isEqual(
                connections[keyName],
                importedConnections[keyName]
            )
        }
    ).map(
        (keyName: string) => {
            let changes = []
            let conn = [
                importedConnections[keyName],
                connections[keyName]
            ]
            if (conn[0].label !== conn[1].label) {
                changes.push({
                    type: 'connection-rename',
                    from: keyName,
                    to: conn[1].label
                })
                dontChange.push(conn[1].id)
            }
            if (conn[0].from === conn[1].to) {
                changes.push({
                    type: 'connection-mirror',
                    from: keyName,
                    to: conn[1].from
                })
                dontChange.push(conn[1].id)
            }
            if (conn[0].labelFrom !== conn[1].labelFrom) {
                changes.push({
                    type: 'connection-cardinality-from',
                    from: keyName,
                    to: conn[1].labelFrom
                })
                dontChange.push(conn[1].id)
            }
            if (conn[0].labelTo !== conn[1].labelTo) {
                changes.push({
                    type: 'connection-cardinality-to',
                    from: keyName,
                    to: conn[1].labelTo
                })
                dontChange.push(conn[1].id)
            }
            if (conn[0].type !== conn[1].type) {
                changes.push({
                    type: 'connection-retype',
                    from: keyName,
                    to: conn[0].type
                })
                dontChange.push(conn[1].id)
            }
            return changes
        }
    )

    let newConnections: any[] = []

    modifiedConnections.forEach(
        (value: any) => {
            if (value) {
                newConnections = newConnections.concat(value)
            }
        }
    )

    Object.keys(importedConnections).filter(
        (key: any) => {
            return !connections[key]
        }
    ).forEach(
        (key: any) => {
            let connection = importedConnections[key]
            if (dontChange.indexOf(connection.id) < 0) {
                newConnections.push({
                    type: 'delete-connection',
                    from: key,
                    to: {id: connection.id}
                })
            }
        }
    )

    return newConnections.filter(
        (value: any) => {
            return value
        }
    )
}

const getDifferencesConnections = (
    nodes: any,
    importedConnections: any,
    connections: any
) => {
    const newConnections = getNewConnections(nodes, importedConnections, connections)
    const modifiedConnections = getModifiedConnections(importedConnections, connections)
    return ([] as any[]).concat(newConnections, modifiedConnections)
}

const ChangesMenu = () => {
    const {
        importedNodes,
        importedConnections,
        nodes,
        connections,
        writeData,
        writeDataLocal,
        currentActivity
    } = React.useContext(EditorData)!

    const nodeChanges = React.useMemo(
        () => getDifferencesNodes(
            importedNodes,
            nodes
        ),
        [nodes, importedNodes]
    )

    const connectionChanges = React.useMemo(
        () => getDifferencesConnections(
            nodes,
            importedConnections,
            connections
        ),
        [nodes, connections, importedConnections]
    )

    const startChangesToPush = () =>{
        if(Object.keys(importedNodes).length > 0) {
            let activity_key = null;
            if (currentActivity != null) {
                activity_key = currentActivity.id;
            }
            return [{
                type: 'retype-activity',
                to:{
                    type:'Activity',
                    retype: "name",
                    name: activityName,
                    action:'',
                    id: activity_key
                },
                nodeKey:uuid.v4(),
                key:null,
                from:null
            }];
        }
        else {
            return [{
                type: 'new-activity',
                name: activityName,
                to:{
                    type:'Activity',
                    name: activityName,
                    action:''
                },
                nodeKey:uuid.v4(),
                key:null,
                from:null
            }];
        }
    }

    const [dialog, setDialog] = React.useState(false)
    const [done, setDone] = React.useState(false)
    const [error, setError] = React.useState(false)
    const { setActivityName,activityName } = React.useContext(EditorData)!
    
    if (currentActivity !== null && currentActivity.name.trim() !== "") {
        setActivityName(currentActivity.name);
    }

    return (
        <div style={{padding: 22, overflowY: 'scroll',textAlign:'center',fontWeight:600}}>
            Activity name:
            <EditableText
                value={activityName}
                onChange={(e) => {
                    setActivityName(e);
                }}
                maxLength={64}
                className='nguml-activity-name'
                disabled={Object.keys(nodeChanges).length === 0}/>
            <HTMLTable
                style={{ width: '100%' }}
                striped
                interactive
            >
                <thead>
                    <th></th>
                    <th>Change</th>
                    <th>Container</th>
                    <th>Data</th>
                    <th></th>
                </thead>
                <tbody>
                {
                    Object.keys(nodeChanges).map(
                        (change: any, index: number) => {
                            if (nodeChanges[index]['type'] === 'new-classifier')
                                return (
                                    <tr>
                                        <td><Icon icon='new-object'/></td>
                                        <td>Add Class</td>
                                        <td>-</td>
                                        <td>{nodeChanges[index]['to']['name']}</td>
                                        <td></td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'delete-property')
                                return (
                                    <tr
                                        style={{
                                            backgroundColor: (
                                                Object.keys(nodeChanges[index]['from']['instances']).length ? 
                                                'red' : '')
                                        }}
                                    >
                                        <td><Icon icon='remove-row-bottom'/></td>
                                        <td>Delete Property</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>+{nodeChanges[index]['to']['name']} : {nodeChanges[index]['to']['type']}</td>
                                        <td>{
                                            Object.keys(nodeChanges[index]['from']['instances']).length ?
                                            <Icon icon='warning-sign' intent='danger'/>
                                            :
                                            <></>
                                        }</td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'new-property')
                                return (
                                    <tr>
                                        <td><Icon icon='add-row-bottom'/></td>
                                        <td>Add Property</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>+{nodeChanges[index]['to']['name']} : {nodeChanges[index]['to']['type']}</td>
                                        <td></td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'delete-method')
                                return (
                                    <tr>
                                        <td><Icon icon='publish-function'/></td>
                                        <td>Delete Method</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>{nodeChanges[index]['to']['name']}() : {nodeChanges[index]['to']['type']}</td>
                                        <td></td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'new-method')
                                return (
                                    <tr>
                                        <td><Icon icon='function'/></td>
                                        <td>Add Method</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>{nodeChanges[index]['to']['name']}() : {nodeChanges[index]['to']['type']}</td>
                                        <td></td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'retype-method')
                                return (
                                    <tr>
                                        <td><Icon icon='function'/></td>
                                        <td>Retype Method</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>{nodeChanges[index]['to']['type']}</td>
                                        <td></td>
                                    </tr>
                                )
                            if (nodeChanges[index]['type'] === 'retype-property')
                                return (
                                    <tr>
                                        <td><Icon icon='exclude-row'/></td>
                                        <td>Retype Property</td>
                                        <td>{nodeChanges[index]['from']['name']}</td>
                                        <td>
                                            {nodeChanges[index]['to']['name']} 
                                            {' -> '} 
                                            {nodeChanges[index]['to']['type']}
                                        </td>
                                        <td></td>
                                    </tr>
                                )
                            else {
                                return <></>
                            }
                        }
                    )
                }
                {
                    Object.keys(connectionChanges).map(
                        (change: any, index: number) => {
                            if (connectionChanges[index]['type'] === 'new-connection')
                                return (
                                    <tr>
                                        <td><Icon icon='new-link'/></td>
                                        <td>Add Connection</td>
                                        <td>{
                                           nodes[
                                               connectionChanges[index]['to']['from']
                                           ].name + ' - ' + nodes[
                                            connectionChanges[index]['to']['to']
                                           ].name
                                        }</td>
                                        <td>{
                                            connectionChanges[index]['to']['labelFrom'] + ' - ' +
                                            connectionChanges[index]['to']['label'] + ' - ' +
                                            connectionChanges[index]['to']['labelTo']
                                        }</td>
                                        <td></td>
                                    </tr>
                                )
                            if (connectionChanges[index]['type'] === 'connection-mirror')
                                return (
                                    <tr>
                                        <td><Icon icon='refresh'/></td>
                                        <td>Mirror Connection</td>
                                        <td>{
                                           nodes[
                                               connections[connectionChanges[index]['from']]['to']
                                           ].name + ' - ' + nodes[
                                            connections[connectionChanges[index]['from']]['from']
                                           ].name
                                        }</td>
                                        <td>{
                                           nodes[
                                               connections[connectionChanges[index]['from']]['from']
                                           ].name + ' - ' + nodes[
                                            connections[connectionChanges[index]['from']]['to']
                                           ].name
                                        }</td>
                                        <td></td>
                                    </tr>
                                )
                            if (connectionChanges[index]['type'] === 'connection-rename')
                            return (
                                <tr>
                                    <td><Icon icon='tag'/></td>
                                    <td>Rename Connection</td>
                                    <td>{
                                       nodes[
                                           connections[connectionChanges[index]['from']]['from']
                                       ].name + ' - ' + nodes[
                                        connections[connectionChanges[index]['from']]['to']
                                       ].name
                                    }</td>
                                    <td>{
                                        connectionChanges[index]['to']
                                    }</td>
                                    <td></td>
                                </tr>
                            )
                            if (connectionChanges[index]['type'] === 'connection-cardinality-from')
                                return (
                                    <tr>
                                        <td><Icon icon='asterisk'/></td>
                                        <td>Change Cardinality</td>
                                        <td>{
                                           nodes[
                                               connections[connectionChanges[index]['from']]['from']
                                           ].name + ' - ' + nodes[
                                            connections[connectionChanges[index]['from']]['to']
                                           ].name
                                        }</td>
                                        <td>{
                                            nodes[
                                                connections[connectionChanges[index]['from']]['from']
                                            ].name + ' : ' + connectionChanges[index]['to']
                                        }</td>
                                        <td></td>
                                    </tr>
                                )
                            if (connectionChanges[index]['type'] === 'connection-cardinality-to')
                                return (
                                    <tr>
                                        <td><Icon icon='asterisk'/></td>
                                        <td>Change Cardinality</td>
                                        <td>{
                                           nodes[
                                               connections[connectionChanges[index]['from']]['from']
                                           ].name + ' - ' + nodes[
                                            connections[connectionChanges[index]['from']]['to']
                                           ].name
                                        }</td>
                                        <td>{
                                            nodes[
                                                connections[connectionChanges[index]['from']]['to']
                                            ].name + ' : ' + connectionChanges[index]['to']
                                        }</td>
                                        <td></td>
                                    </tr>
                                )
                            if (connectionChanges[index]['type'] === 'connection-retype')
                                return (
                                    <tr>
                                        <td><Icon icon='wrench'/></td>
                                        <td>Retype Connection</td>
                                        <td>{
                                           nodes[
                                               connections[connectionChanges[index]['from']]['from']
                                           ].name + ' - ' + nodes[
                                            connections[connectionChanges[index]['from']]['to']
                                           ].name
                                        }</td>
                                        <td>{
                                            connectionChanges[index]['to'][0].toUpperCase() +
                                            connectionChanges[index]['to'].substring(1) +
                                            ' -> ' +
                                            connections[connectionChanges[index]['from']]['type'][0].toUpperCase() + 
                                            connections[connectionChanges[index]['from']]['type'].substring(1)
                                        }</td>
                                        <td></td>
                                    </tr>
                                )
                            if (connectionChanges[index]['type'] === 'delete-connection')
                                return (
                                    <tr>
                                        <td><Icon icon='graph-remove'/></td>
                                        <td>Delete Connection</td>
                                        <td>{
                                           nodes[
                                               importedConnections[connectionChanges[index]['from']]['from']
                                           ].name
                                        }</td>
                                        <td>-</td>
                                        <td></td>
                                    </tr>
                                )
                            else
                                return <>{connectionChanges[index]['type']}</>
                        }
                    )
                }
                </tbody>
            </HTMLTable>
            {
                nodeChanges.length || connectionChanges.length ?
                    <>
                        <ButtonGroup style={{padding: 32}} fill>
                            <Button 
                                onClick={() => setDialog(true)}
                                intent="primary"
                            >Push to Backend</Button>
                        </ButtonGroup>
                        <ButtonGroup style={{padding: 32}} fill>
                            <Button
                                intent="primary"
                                onClick={() => {
                                    let changesToPush: any[] = []
                                    changesToPush = changesToPush.concat(
                                        nodeChanges.map(
                                            (value: any) => {
                                                return {
                                                    ...value,
                                                    key: value['from'],
                                                    from: nodes[value.from]
                                                }
                                            }
                                        )
                                    )
                                    changesToPush = changesToPush.concat(
                                        connectionChanges.map(
                                            (value: any) => {
                                                return {
                                                    ...value,
                                                    key: value['from'],
                                                    from: connections[value.from]
                                                }
                                            }
                                        )
                                    )
                                    writeDataLocal(changesToPush, setDone, setError)
                                }}
                            >Save locally</Button>
                        </ButtonGroup>
                        <Alert
                            className='bp3-dark'
                            intent='warning'
                            confirmButtonText='Yes'
                            icon='cloud-upload'
                            cancelButtonText='No'
                            isOpen={dialog}
                            onClose={() => setDialog(false)}
                            onConfirm={() => {
                                let changesToPush: any[] = startChangesToPush()
                                changesToPush = changesToPush.concat(
                                    nodeChanges.map(
                                        (value: any) => {
                                            return {
                                                ...value,
                                                key: value['from'],
                                                from: nodes[value.from]
                                            }
                                        }
                                    )
                                )
                                changesToPush = changesToPush.concat(
                                    connectionChanges.map(
                                        (value: any) => {
                                            return {
                                                ...value,
                                                key: value['from'],
                                                from: connections[value.from]
                                            }
                                        }
                                    )
                                )
                                writeData(changesToPush, setDone, setError)
                            }}
                        >
                            Are you sure you want to push the changes,
                            this will overwrite the existing application
                            structure and is an irreversible action.
                        </Alert>
                    </>
                    :
                    <div style={{margin: 32, textAlign: 'center', padding: 32, fontSize: 21}}>
                        <Icon 
                            style={{
                                marginRight: 16
                            }}
                            icon='clean' 
                            intent='success'
                            iconSize={24}
                        />
                        No changes to push
                    </div>
            }
            <Alert
                icon="clean"
                className='bp3-dark'
                intent="success"
                isOpen={done}
                onClose={() => setDone(false)}
            >
                Successfully pushed to backend!
            </Alert>
            <Alert
                icon="clean"
                className='bp3-dark'
                intent="danger"
                isOpen={error}
                onClose={() => setError(false)}
            >
                Couldn't push to backend!
            </Alert>
        </div>
    )
}

export default ChangesMenu
