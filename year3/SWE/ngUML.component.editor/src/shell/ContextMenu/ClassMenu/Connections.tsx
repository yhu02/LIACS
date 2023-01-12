import React from 'react'
import EditorData from '../../../hooks/editorData'
import { 
    ControlGroup,
    Button,
    HTMLSelect,
    IOptionProps
} from '@blueprintjs/core'
import * as uuid from 'uuid'
import Connection from './Connection'

interface IConnectionsMenu {
    node: string
}

export const Connections: React.FC<IConnectionsMenu> = ({node}) => {
    const {
        nodes,
        connections,
        setConnections
    } = React.useContext(EditorData)!

    const [
        options,
        setOptions
    ] = React.useState<IOptionProps[]>([])

    const [
        newConnection,
        setNewConnection
    ] = React.useState('')

    const [
        newType,
        setNewType
    ] = React.useState('')

    React.useEffect(() => {
        let possibleNodes = Object.keys(nodes).filter(
            (e) => {
                return e !== node;
            }
        )

        setOptions(possibleNodes.map(
            (keyName: string, index: number) => {
                return {
                    label: nodes[keyName].name,
                    value: keyName
                }
            }
        ))
    }, [nodes, node, connections])

    const createConnection = () => {
        let nType = newType !== '' ? newType : 'generalization'

        let labels = () => {
            if (nType === 'generalization') {
                return {
                    from: '*', 
                    to: '*'
                }
            } else if (nType === 'composition') {
                return {
                    from: '1',
                    to: '*'
                }
            } else if (nType === 'composition') {
                return {
                    from: '1',
                    to: '*'
                }
            } else {
                return {
                    from: '1',
                    to: '1'
                }
            }
        }

        let connection = {
            name: '',
            type: newType !== '' ? newType : 'generalization',
            from: node,
            to: newConnection !== '' ? newConnection : String(options[0].value),
            label: 'Connection',
            labelFrom: labels().from,
            labelTo: labels().to
        }
        setConnections({
            ...connections,
            [uuid.v4()]: connection
        })
        setNewConnection('')
        setNewType('')
    }

    return (
        <>
            <div className="nguml-context-connection">
                <div>
                    {
                        Object.keys(connections).map((keyName: string, index: number) => {
                            if (
                                connections[keyName].from === node || 
                                connections[keyName].to === node
                            ) {
                                let connection = connections[keyName]
                                return (
                                    <Connection
                                        connectionId={keyName}
                                        connection={connection}
                                        fromNode={nodes[connection.from]}
                                        toNode={nodes[connection.to]}
                                        connections={connections}
                                        setConnections={setConnections}
                                    />
                                )
                            } else { return <>{null}</> }
                        })
                    }
                </div>
                {
                    options.length > 0 ?
                    <ControlGroup fill>
                        <HTMLSelect
                            options={options}
                            value={newConnection}
                            onChange={(e) => setNewConnection(e.target.value)}
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
                            value={newType}
                            onChange={(e) => setNewType(e.target.value)}
                        />
                        <Button 
                            intent="success"
                            icon="link"
                            onClick={() => createConnection()}
                        />
                    </ControlGroup>
                    :
                    <></>
                }
            </div>
        </>
    )
}

export default Connections