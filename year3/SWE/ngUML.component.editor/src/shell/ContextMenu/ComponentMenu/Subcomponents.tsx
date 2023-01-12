import React from 'react'
import EditorData from '../../../hooks/editorData'
import {
    HTMLSelect,
    ControlGroup,
    InputGroup,
    Alert,
    Button,
} from '@blueprintjs/core'
import AceEditor from 'react-ace'

import 'ace-builds/src-noconflict/mode-python'
import 'ace-builds/src-noconflict/theme-monokai'
import 'ace-builds/src-noconflict/snippets/python'
import 'ace-builds/src-noconflict/ext-language_tools'

interface ISubcomponent {
    name: string,
    type: string
}

interface ISubcomponents {
    node: string
}


const Subcomponents: React.FC<ISubcomponents> = ({ node }) => {
    const [newType, setNewType] = React.useState<string>('string')
    const [newSubcomponent, setNewSubcomponent] = React.useState<string>('')
    const { nodes, setNodes, addNode } = React.useContext(EditorData)!
    const subcomponents: any = nodes[node]['data']['subcomponents'] ?? []
    const [delprompt, setDelprompt] = React.useState(false)

    const setSubcomponents = (newSubcomponents: any) => {
        setNodes({
            ...nodes,
            [node]: {
                ...nodes[node],
                data: {
                    ...nodes[node]['data'],
                    subcomponents: newSubcomponents,
                },
            },
        })
    }

    const addNewSubcomponent = () => {
        if (newType && newSubcomponent) {
            let toAdd: ISubcomponent = {
                name: newSubcomponent,
                type: newType

            }
            subcomponents.push(toAdd)
            setSubcomponents(subcomponents)
            setNewSubcomponent('')
            setNewType('string')
            //addNode(newSubcomponent)
        }
    }

    const editSubcomponent = (index: number, field: string, value: string) => {
        if (subcomponents[index]) {
            if (field === 'name') {
                subcomponents[index]['name'] = value
                setSubcomponents(subcomponents)
            } else if (field === 'code') {
                subcomponents[index]['code'] = value
                setSubcomponents(subcomponents)
            }
        }
    }

    const deleteSubcomponent = (index: number) => {
        delete subcomponents[index]
        subcomponents.splice(index, 1)
        setSubcomponents(subcomponents)
    }

    return (
        <div
            style={{
                padding: '8px 0',
                width: '100%',
                borderTop: 'solid 1px #666',
            }}
        >
            {subcomponents.map((value: ISubcomponent, index: number) => {
                if (value) {
                    return (
                        <div
                            style={{
                                borderBottom: 'solid 1px #666',
                                margin: '8px',
                            }}
                        >
                            <ControlGroup
                                style={{ margin: '2px 0 2px 0' }}
                                fill
                            >
                                <InputGroup
                                    placeholder="subcomponentName"
                                    value={value.name}
                                    onChange={(e) => {
                                        editSubcomponent(
                                            index,
                                            'name',
                                            e.target.value
                                        )
                                    }}
                                />
                                <Button
                                    intent="danger"
                                    icon="delete"
                                    onClick={() => setDelprompt(true)}
                                />
                                <Alert
                                    className="bp3-dark"
                                    intent="danger"
                                    confirmButtonText="Yes"
                                    icon="delete"
                                    cancelButtonText="No"
                                    isOpen={delprompt}
                                    onClose={() => setDelprompt(false)}
                                    onConfirm={() => {
                                        deleteSubcomponent(index)
                                    }}
                                >
                                    Are you sure you want to delete the
                                    subcomponent, this is an irreversible
                                    action.
                                </Alert>
                            </ControlGroup>
                        </div>
                    )
                } else {
                    return <></>
                }
            })}
            <div style={{ margin: '8px' }}>
                <ControlGroup fill>
                    <HTMLSelect
                        options={[
                            {
                                label: 'Kies een optie',
                                value: 'handler',
                            },
                            {
                                label: 'Component',
                                value: 'Component',
                            },
                            {
                                label: 'Class',
                                value: 'Class',
                            },
                            {
                                label: 'Enum',
                                value: 'Enum',
                            },
                            {
                                label: 'Initial',
                                value: 'Initial',
                            },
                            {
                                label: 'Action',
                                value: 'Action',
                            },
                            {
                                label: 'Decision',
                                value: 'Decision',
                            },
                            {
                                label: 'Merge',
                                value: 'Merge',
                            },
                            {
                                label: 'Fork',
                                value: 'Fork',
                            },
                            {
                                label: 'Join',
                                value: 'Join',
                            },
                            {
                                label: 'ActivityFinal',
                                value: 'ActivityFinal',
                            },
                            {
                                label: 'FlowFinal',
                                value: 'FlowFinal',
                            },
                        ]}
                        value={node.type}
                        onChange={(e) => setNewSubcomponent(e.target.value)}
                    />
                    <Button
                        intent="success"
                        icon="add"
                        onClick={() => addNewSubcomponent()}
                    />
                </ControlGroup>
            </div>
        </div>
    )
}
export default Subcomponents
