import React from 'react'
import ClassNode from '../../../diagram/ClassNode'
import { 
    EditableText,
    ControlGroup,
    Button,
    Alert,
    HTMLSelect
} from '@blueprintjs/core'

interface IConnection {
    connectionId: string;
    connection: any;
    connections: any;
    setConnections: (value: any) => void;
    fromNode: any;
    toNode: any;
}

const Connection : React.FC<IConnection> = ({
    connectionId,
    connection,
    connections,
    setConnections,
    fromNode,
    toNode
}) => {
    const [delprompt, setDelprompt] = React.useState(false);

    return (
        <>
            <div
                className='nguml-context-connection-item'
            >
                <div style={{transform: 'scale(0.7)', transformOrigin: 'left'}}>
                    <ClassNode
                        node={fromNode}
                    />
                </div>
                <EditableText
                    value={connection.label}
                    onChange={(target) => {
                        setConnections({
                            ...connections,
                            [connectionId]: {
                                ...connection,
                                label: target
                            }
                        })
                    }}
                />
                <div style={{transform: 'scale(0.7)', transformOrigin: 'right'}}>
                    <ClassNode
                        node={toNode}
                    />
                </div>
            </div>
            <div
                className='nguml-context-connection-options'
            >
                <ControlGroup fill>
                    <Button
                        intent='primary'
                        icon='refresh'
                        onClick={() => {
                            setConnections({
                                ...connections,
                                [connectionId]: {
                                    ...connection,
                                    from: connection.to,
                                    to: connection.from,
                                    labelFrom: connection.labelTo,
                                    labelTo: connection.labelFrom
                                }
                            })
                        }}
                    />
                    <HTMLSelect
                        options={[
                            '1',
                            '*'
                        ]}
                        value={connection.labelFrom}
                        onChange={(e) => {
                            setConnections({
                                ...connections,
                                [connectionId]: {
                                    ...connection,
                                    labelFrom: e.target.value
                                }
                            })
                        }}
                    />
                    <HTMLSelect
                        options={[
                            {
                                label: 'Generalization',
                                value: 'generalization'
                            },
                            {
                                label: 'Association',
                                value: 'association'
                            },
                            {
                                label: 'Composition',
                                value: 'composition'
                            },
                        ]}
                        value={connection.type}
                        onChange={(e) => {
                            setConnections({
                                ...connections,
                                [connectionId]: {
                                    ...connection,
                                    type: e.target.value
                                }
                            })
                        }}
                    />
                    <HTMLSelect
                        options={[
                            '1',
                            '*'
                        ]}
                        value={connection.labelTo}
                        onChange={(e) => {
                            setConnections({
                                ...connections,
                                [connectionId]: {
                                    ...connection,
                                    labelTo: e.target.value
                                }
                            })
                        }}
                    />
                    <Button
                        intent='danger'
                        icon='delete'
                        onClick={() => setDelprompt(true)}
                    />
                    <Alert
                        className='bp3-dark'
                        intent='danger'
                        confirmButtonText='Yes'
                        icon='delete'
                        cancelButtonText='No'
                        isOpen={delprompt}
                        onClose={() => setDelprompt(false)}
                        onConfirm={() => {
                            let newConnections = {...connections}
                            delete newConnections[connectionId]
                            setConnections(newConnections)
                        }}
                    >
                        Are you sure you want to delete the connection,
                        this is an irreversible action.
                    </Alert>
                </ControlGroup>
            </div>
        </>
    )
}

export default Connection
