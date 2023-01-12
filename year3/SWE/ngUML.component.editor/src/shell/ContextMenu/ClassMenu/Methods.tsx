import React from 'react'
import EditorData from '../../../hooks/editorData'
import {
    HTMLSelect,
    ControlGroup,
    InputGroup,
    Button
} from '@blueprintjs/core'
import AceEditor from "react-ace";

import "ace-builds/src-noconflict/mode-python";
import "ace-builds/src-noconflict/theme-monokai";
import "ace-builds/src-noconflict/snippets/python";
import "ace-builds/src-noconflict/ext-language_tools";

interface IMethod {
    name: string;
    type: string;
    code: string;
}

interface IMethods {
    node: string
}

const Methods: React.FC<IMethods> = ({node}) => {
    const [newType, setNewType] = React.useState<string>('string')
    const [newMethod, setNewMethod] = React.useState<string>('')
    const {nodes, setNodes} = React.useContext(EditorData)!
    const methods : any = nodes[node]['data']['methods'] ?? []

    const setMethods = (newMethods: any) => {
        setNodes({
            ...nodes,
            [node]: {
                ...nodes[node],
                data: {
                    ...nodes[node]['data'],
                    methods: newMethods
                }
            }
        });
    }
    
    const addNewMethod = () => {
        if (newType && newMethod) {
            let toAdd: IMethod = {
                name: newMethod,
                type: newType,
                code: ''
            }
            methods.push(toAdd)
            setMethods(methods)
            setNewMethod('')
            setNewType('string')
        }
    }

    const editMethod = (
        index: number, 
        field: string, 
        value: string
    ) => {
        if (methods[index]) {
            if (field === 'name') {
                methods[index]['name'] = value
                setMethods(methods)
            } else if (field === 'type') {
                methods[index]['type'] = value
                setMethods(methods)
            } else if (field === 'code') {
                methods[index]['code'] = value
                setMethods(methods)
            }
        }
    }

    const deleteMethod = (
        index: number
    ) => {
        delete methods[index]
        methods.splice(index, 1)
        setMethods(methods)
    }

    return (
        <div style={{
            padding: '8px 0',
            width: '100%',
            borderTop: 'solid 1px #666'
        }}>
            {methods.map((value: IMethod, index: number) => {
                if (value) {
                    return (
                        <div style={{
                            borderBottom: 'solid 1px #666',
                            margin: '8px'
                        }}>
                            <ControlGroup 
                                style={{margin: '2px 0 2px 0'}}
                                fill
                            >
                                <InputGroup
                                    placeholder='methodName'
                                    value={value.name}
                                    onChange={(e) => {editMethod(
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
                                    onChange={(e) => editMethod(
                                        index,
                                        'type',
                                        e.target.value
                                    )}
                                />
                                <Button
                                    intent='danger'
                                    icon='delete'
                                    onClick={() => deleteMethod(index)}
                                />
                            </ControlGroup>
                                <AceEditor
                                    mode="python"
                                    theme="monokai"
                                    onChange={(value) => editMethod(
                                        index,
                                        'code',
                                        value
                                    )}
                                    style={{
                                        height: 128,
                                        margin: 8,
                                        width: 338
                                    }}
                                    fontSize={10}
                                    showPrintMargin={true}
                                    showGutter={true}
                                    highlightActiveLine={true}
                                    value={value.code}
                                    setOptions={{
                                        enableBasicAutocompletion: true,
                                        enableLiveAutocompletion: true,
                                        enableSnippets: true,
                                        showLineNumbers: true,
                                        tabSize: 2,
                                    }}/>
                        </div>
                    )  
                } else { return <></> }
            })}
            <div 
                style={{margin: '8px'}}
            >
                <ControlGroup 
                    fill
                >
                    <InputGroup
                        placeholder='newMethod'
                        value={newMethod}
                        onChange={(e) => setNewMethod(e.target.value)}
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
                        onClick={() => addNewMethod()}
                    />
                </ControlGroup>
            </div>
        </div>
    )
}

export default Methods