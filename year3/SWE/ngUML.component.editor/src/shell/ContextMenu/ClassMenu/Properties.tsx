import React from 'react'
import EditorData from '../../../hooks/editorData'
import {
    HTMLSelect,
    ControlGroup,
    InputGroup,
    Button
} from '@blueprintjs/core'

interface IProperty {
    name: string;
    type: string;
}

interface IProperties {
    node: string
}

const Properties: React.FC<IProperties> = ({node}) => {
    const [newType, setNewType] = React.useState<string>('string')
    const [newProperty, setNewProperty] = React.useState<string>('')
    const {nodes, setNodes} = React.useContext(EditorData)!
    const properties : any = nodes[node]['data']['properties'] ?? []

    const setProperties = (newProperties: any) => {
        setNodes({
            ...nodes,
            [node]: {
                ...nodes[node],
                data: {
                    ...nodes[node]['data'],
                    properties: newProperties
                }
            }
        })
    }
    
    const addNewProperty = () => {
        if (newType && newProperty) {
            let toAdd: IProperty = {
                name: newProperty,
                type: newType
            }
            properties.push(toAdd)
            setProperties(properties)
            setNewProperty('')
            setNewType('string')
        }
    }

    const editProperty = (
        index: number, 
        field: string, 
        value: string
    ) => {
        if (properties[index]) {
            if (field === 'name') {
                properties[index]['name'] = value
                setProperties(properties)
            } else if (field === 'type') {
                properties[index]['type'] = value
                setProperties(properties)
            }
        }
    }

    const deleteProperty = (
        index: number
    ) => {
        delete properties[index]
        properties.splice(index, 1)
        setProperties(properties)
    }

    return (
        <div style={{
            margin: '8px',
            width: '100%',
            padding: '16px 0 0 0',
            borderTop: 'solid 1px #666'
        }}>
            {properties.map((value: IProperty, index: number) => {
                if (value) {
                    return (
                        <div style={{margin: '8px'}}>
                            <ControlGroup 
                                style={{margin: '2px 0 2px 0'}}
                                fill
                            >
                                <InputGroup
                                    placeholder='propertyName'
                                    value={value.name}
                                    onChange={(e) => {editProperty(
                                        index,
                                        'name',
                                        e.target.value
                                    )}}
                                />
                                <HTMLSelect
                                    options={[
                                        'string',
                                        'int',
                                        'bool'
                                    ]}
                                    value={value.type}
                                    onChange={(e) => editProperty(
                                        index,
                                        'type',
                                        e.target.value
                                    )}
                                />
                                <Button
                                    intent='danger'
                                    icon='delete'
                                    onClick={() => deleteProperty(index)}
                                />
                            </ControlGroup>
                        </div>
                    )    
                } else { return <></> }
            })}
            <div style={{ margin: '8px'}}>
                <ControlGroup 
                    fill
                    style={{margin: '2px 0 2px 0'}}
                >
                    <InputGroup
                        placeholder='newProperty'
                        value={newProperty}
                        onChange={(e) => setNewProperty(e.target.value)}
                    />
                    <HTMLSelect
                        options={[
                            'string',
                            'int',
                            'bool'
                        ]}
                        value={newType}
                        onChange={(e) => setNewType(e.target.value)}
                    />
                    <Button
                        intent='success'
                        icon='add'
                        onClick={() => addNewProperty()}
                    />
                </ControlGroup>
            </div>
        </div>
    )
}

export default Properties