// Description: Context menu for the component modeler

// Import the necessary libraries
import { baseURL } from 'hooks/primitiveState'
import { MarkerType } from 'reactflow';
import {
    projectAtom,
    systemAtom,
    listAtom,
    relationsAtom,
    countAtom
} from 'features/diagram/state'
import { useAtom, useAtomValue } from 'jotai'
import React from 'react'
import styled from 'styled-components'
import {
    HTMLSelect,
    Collapse,
    Callout,
    ControlGroup,
    InputGroup,
    Alert,
    Button,
} from '@blueprintjs/core'
import './index.css'

// Define the props
type ContextProps = {}

// Define Box for styling
const Box = styled(Callout)`
    width: 290px;
    height: 100%;
    
`
// Code for the context menu
export const Context: React.FC<ContextProps> = () => {
    // Get the project and system   
    const project = useAtomValue(projectAtom)
    const system = useAtomValue(systemAtom)
    // Updating counter triggers useEffect to refetch nodes relations and lists
    const [counter, setCounter] = useAtom(countAtom)
    const [listCom] = useAtom(listAtom)
    // Get the base URL
    const [_baseURL] = useAtom(baseURL)
    // Open and close the context menu
    const [open, setOpen] = React.useState(false)
    const [listRel] = useAtom(relationsAtom)

    // Add Component
    const [addCompParentId, setAddCompParentId] = React.useState<number | string>('')
    const [addCompName, setAddCompName] = React.useState<any>()
    // Edit Component
    const [editCompId, setEditCompId] = React.useState<number | null>()
    const [editCompName, setEditCompName] = React.useState<any>()
    const [editCompWidth, setEditCompWidth] = React.useState<any>()
    const [editCompHeight, setEditCompHeight] = React.useState<any>()
    // Delete Component
    const [delCompId, setDelCompId] = React.useState<number | null>()
    const [delCompPromt, setDelCompPrompt] = React.useState(false)
    // Add Lollipop
    const [addLolName, setAddLolName] = React.useState<any>()
    const [addLolParentId, setAddLolParentId] = React.useState<number | null>(null)
    // Add Required Interface
    const [addRequiredInterfaceParentId, setAddRequiredInterfaceParentId] = React.useState<number | null>(null)
    const [addRequiredInterfaceName, setAddRequiredInterfaceName] = React.useState<any>()
    const [addRequiredInterfaceType, setAddRequiredInterfaceType] = React.useState<any>('socketLeft')
    // Add Relation
    const [addRelSource, setAddRelSource] = React.useState<number | null>()
    const [addRelTarget, setAddRelTarget] = React.useState<number | null>()
    const [addRelName, setAddRelName] = React.useState<any>()
     // Add Pointer
     const [addPointerSource, setAddPointerSource] = React.useState<number | null>()
     const [addPointerTarget, setAddPointerTarget] = React.useState<number | null>()
     const [addPointerName, setAddPointerName] = React.useState<any>()
    // Edit Relation
    const [editRelId, setEditRelId] = React.useState<number | null>()
    const [editRelName, setEditRelName] = React.useState<any>()
    // Delete Relation
    const [delRelId, setDelRelId] = React.useState<number | null>()
    const [delRelPrompt, setDelRelPrompt] = React.useState(false)
    //Switch Relation
    const [switchId, setSwitchId] = React.useState<number | null>()
    //Switch arrow direction dependencies
    const [switchArrowId, setSwitchArrowId] = React.useState<number | null>()
     //Bind to component Id Relation
     const [bindReqId, setBindReqId] = React.useState<any>()
     const [bindProId, setBindProId] = React.useState<any>()

    // Endpoint to edit a component name
    const handleClickEditCompName = () => {

        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_component/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({ id: editCompId, data: { label: editCompName } })
        })
        setCounter(counter+1)
    }

     // Endpoint to edit a component's size
    const handleClickEditCompSize = () => {

        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_component/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({ id: editCompId, style: { width: editCompWidth, height: editCompHeight, border: "1px solid #777"} })
        })
        setCounter(counter+1)
    }

    // Endpoint to delete a component
    const handleClickDelComp = () => {
        setCounter(counter+1)

        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/delete_component/`, {
            method: 'DELETE',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({ id: delCompId })
        })
    }

    // Endpoint to add a component
    const handleClickAddComp = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/add_component/`, {

            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                type: "custom",
                data: {
                    label: addCompName
                },
                position: { x: 3, y: 3 },
                style: {
                    width: 200,
                    height: 200,
                    backgroundColor: "rgba(240, 240, 240, 0.3)",
                    border: "1px solid #777"
                },
                model: Number(system),
                parentNode: addCompParentId,
                extent: 'parent',
                className: "light",
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to add a provided interface
    const handleClickAddLol = () => {

        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/add_component/`, {

            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                type: "circle",
                data: {
                    label: addLolName
                },
                position: { x: 3, y: 3 },
                style: {
                    width: 20,
                    height: 20,
                    backgroundColor: "rgba(255, 0, 0, 0)",
                    border: ""
                },
                model: Number(system),
                parentNode: addLolParentId,
                className: "light",
                target: Number(bindProId)
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to add a required interface
    const handleAddRequiredInterface = () => {

        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/add_component/`, {

            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                type: addRequiredInterfaceType,
                data: {
                    label: addRequiredInterfaceName
                },
                position: { x: 3, y: 3 },
                style: {
                    width: 20,
                    height: 20,
                    backgroundColor: "rgba(255, 0, 0, 0)",
                    border: ""
                },
                model: Number(system),
                parentNode: addRequiredInterfaceParentId,
                className: "light",
                target: Number(bindReqId)
            }),
        })
        setCounter(counter+1)
    }
   
    // Endpoint to add an edge
    const handleClickAddRel = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/add_relation/`, {

            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                source: addRelSource,
                label: addRelName,
                target: addRelTarget,
                animated: "False",
                dependency: false
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to add a dependency
    const handleClickAddPointer = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/add_relation/`, {

            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                source: addPointerSource,
                label: addPointerName,
                target: addPointerTarget,
                animated: true,
                markerStart: {
                    type: MarkerType.Arrow,
                    width: 30,
                    height: 30,
                },
                dependency: true
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to edit a relation
    const handleClickSwitchArrow = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_relation/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                id: switchArrowId,
                markerStart: {
                    type: MarkerType.Arrow,
                    width: 30,
                    height: 30,
                },
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to edit a relation
    const handleClickSwitchArrowBack = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_relation/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                id: switchArrowId,
                markerEnd: {
                    type: MarkerType.Arrow,
                    width: 30,
                    height: 30,
                },
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to switch direction of arrow
    const handleClickEditRel = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_relation/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                id: editRelId,
                label: editRelName
            }),
        })
        setCounter(counter+1)
    }
    
    // Endpoint to switch target and source of two components in a relation
    const handleClickSwitch = () => {
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/switch_port/`, {

            method: 'PUT',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                id: switchId,
            }),
        })
        setCounter(counter+1)
    }

    // Endpoint to delete a relation
    const handleClickDelRel = () => {
        setCounter(counter+1)
        fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/delete_relation/`, {

            method: 'DELETE',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify({
                id: delRelId
            }),
        })
        setTimeout(() => {
            setCounter(counter+1)
        }, 1000);
    }

    // Adds a no parent option for add component
    const option1 = listCom.map((component: any) => {
        return { label: component[0] , value: component[1] }
    })
    const options = option1
    const optionsRequiredInterface = [{ label: 'Right', value: 'socketRight' },{ label: 'Up', value: 'socketUp' },{ label: 'Down', value: 'socketDown' }]
    const listDependency = listRel.filter((obj: any )=> {
        return obj[2] === 'Dependency';
    });

    //Here the actual contextmenu gets rendered
    return (
        <div>
            <Button
                onClick={() => setOpen((open) => !open)}
                intent={open ? 'warning' : 'primary'}
            >
                {open ? 'Hide' : 'Show'} context
            </Button>
            <Collapse isOpen={open}>
                <Box>
                <div style={{ margin: '15px' }}>
                 
                <p>Add Component</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    options
                                }

                                onChange={selected => {
                                    setAddCompParentId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" >No parent</option>
                            </HTMLSelect>
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setAddCompName(selected.target.value)
                                }}
                            />
                            <Button
                                intent="success"
                                icon="add"
                                onClick={() => {handleClickAddComp(); setCounter(counter+1);} }
                            />
                            
                        </ControlGroup>
                        <p>Edit Component</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: "Name: " + component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setEditCompId(Number(selected.target.value))
                                    setDelCompId(Number(selected.target.value))
                                }}
                            >
                                
                                <option value="DEFAULT" disabled>Component</option>
                            </HTMLSelect>
                            <Button
                                data-testid="delete-subcomponent-button"
                                intent="danger"
                                text="Delete"
                                onClick={() => {setDelCompPrompt(true); setCounter(counter+1);}}
                            />
                            <Alert
                                className="bp3-dark"
                                intent="danger"
                                confirmButtonText="Yes"
                                icon="delete"
                                cancelButtonText="No"
                                isOpen={delCompPromt}
                                onClose={() => setDelCompPrompt(false)}
                                onConfirm={() => {
                                    handleClickDelComp(); setCounter(counter+1);
                                }}
                            >
                                Are you sure you want to delete the
                                component? This is an irreversible
                                action.
                            </Alert>
                            </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setEditCompName(selected.target.value);
                                }}
                            />
                            
                            <Button
                                intent="success"
                                icon="refresh"
                                onClick={() => {handleClickEditCompName(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Width"
                                onChange={(selected) => {
                                    setEditCompWidth(Number(selected.target.value))
                                }}
                            />
                            <InputGroup
                                placeholder="Height"
                                onChange={(selected) => {
                                    setEditCompHeight(Number(selected.target.value))
                                }}
                            />
                            <Button
                                intent="success"
                                icon="refresh"
                                onClick={() => {handleClickEditCompSize(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                <p>Add Provided Interface</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    options
                                }

                                onChange={selected => {
                                    setAddLolParentId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT">No parent</option>
                            </HTMLSelect>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: "Name: " + component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setBindProId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Component</option>
                            </HTMLSelect>
                        </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setAddLolName(selected.target.value)
                                }}
                            />
                            <Button
                                data-testid="add-provided-interface-button"
                                intent="success"
                                icon="add"
                                onClick={() => {handleClickAddLol(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Add Required Interface</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    options
                                }

                                onChange={selected => {
                                    setAddRequiredInterfaceParentId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT">No parent</option>
                            </HTMLSelect>
                        
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: "Name: " + component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setBindReqId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Component</option>
                            </HTMLSelect>
                        </ControlGroup>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    optionsRequiredInterface
                                }

                                onChange={selected => {
                                    setAddRequiredInterfaceType(selected.target.value)
                                }}
                            >
                                <option value="DEFAULT">Left</option>
                            </HTMLSelect>
                        
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setAddRequiredInterfaceName(selected.target.value)
                                }}
                            />
                            <Button
                                data-testid="add-required-interface-button"
                                intent="success"
                                icon="add"
                                onClick={() => {handleAddRequiredInterface(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Add Relation</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label:  component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setAddRelTarget(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Source</option>
                            </HTMLSelect>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: component[0] , value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setAddRelSource(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Target</option>
                            </HTMLSelect>
                        </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setAddRelName(selected.target.value)
                                }}
                            />
                            <Button
                                data-testid="add-edge-button"
                                intent="success"
                                icon="add"
                            onClick={() => {handleClickAddRel(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Add Dependency</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setAddPointerTarget(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Source</option>
                            </HTMLSelect>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listCom.map((component: any) => {
                                        return { label: component[0], value: component[1] }
                                    })
                                }
                                onChange={selected => {
                                    setAddPointerSource(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Target</option>
                            </HTMLSelect>
                        </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Name"
                                onChange={(selected) => {
                                    setAddPointerName(selected.target.value)
                                }}
                            />
                            <Button
                                data-testid="add-edge-button"
                                intent="success"
                                icon="add"
                            onClick={() => {handleClickAddPointer(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Edit Edge</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listRel.map((e: any) => {
                                        return { label: e[1] + ' [' + e[2] + ']', value: e[0] }
                                    })
                                }
                                onChange={selected => {
                                    setEditRelId(Number(selected.target.value));
                                    setDelRelId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Edge</option>
                            </HTMLSelect>
                            <Button
                                data-testid="delete-relation-button"
                                intent="danger"
                                text="Delete"
                                onClick={() => setDelRelPrompt(true)}
                            />
                            <Alert
                                className="bp3-dark"
                                intent="danger"
                                confirmButtonText="Yes"
                                icon="delete"
                                cancelButtonText="No"
                                isOpen={delRelPrompt}
                                onClose={() => setDelRelPrompt(false)}
                                onConfirm={() => {
                                    handleClickDelRel(); setCounter(counter+1);
                                }}
                            >
                                Are you sure you want to delete the
                                relation? This is an irreversible
                                action.
                            </Alert>
                        </ControlGroup>
                        <ControlGroup fill>
                            <InputGroup
                                placeholder="Name"
                                // value={subcomponentName}
                                onChange={(selected) => {
                                    setEditRelName(selected.target.value);
                                }}
                            />
                            <Button
                                intent="success"
                                icon="refresh"
                                onClick={() => {handleClickEditRel(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Switch ports</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listRel.map((e: any) => {
                                        return { label: e[1] + ' [' + e[2] + ']', value: e[0] }
                                    })
                                }
                                onChange={selected => {
                                    setSwitchId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Edge</option>
                            </HTMLSelect>
                            <Button
                                intent="success"
                                icon="refresh"
                                onClick={() => {handleClickSwitch(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                        <p>Choose arrow side</p>
                        <ControlGroup fill>
                            <HTMLSelect
                                defaultValue="DEFAULT"
                                options={
                                    listDependency.map((e: any) => {
                                            return { label: e[1] , value: e[0] }
                                    })
                                }
                                onChange={selected => {
                                    setSwitchArrowId(Number(selected.target.value))
                                }}
                            >
                                <option value="DEFAULT" disabled>Edge</option>
                            </HTMLSelect>
                            <Button
                                intent="success"
                                text="Start"
                                onClick={() => {handleClickSwitchArrowBack(); setCounter(counter+1);}}
                            />
                            <Button
                                intent="success"
                                text="End"
                                onClick={() => {handleClickSwitchArrow(); setCounter(counter+1);}}
                            />
                        </ControlGroup>
                </div>
                </Box>

            </Collapse>
        </div>
    )

}

export default Context
