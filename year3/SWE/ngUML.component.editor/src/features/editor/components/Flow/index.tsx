import React from 'react'
import ReactFlow, {
    applyNodeChanges,
    Background,
    ConnectionLineType,
    Controls,
    EdgeChange,
    NodeChange,
} from 'reactflow'
import 'reactflow/dist/style.css'
import styled from 'styled-components'
import { mapNodes, mapConnections } from '@nguml-editor/editor/utils'
import LegacyNode from '../LegacyNode'
import { pushNodeChanges, usePutModel } from 'features/editor/mutations/model'
import { useAtom } from 'jotai'
import { baseURL as baseURLRef } from 'hooks/primitiveState'
import { useParams } from 'react-router-dom'
import { useQueryClient } from '@tanstack/react-query'

type FlowProps = {
    nodes: any
    connections: any
}

const WrapBox = styled.div`
    inset: 0;
    position: absolute;
    padding: 4em;
`

export const Flow: React.FC<FlowProps> = (props) => {
    const { project, system, model } = useParams()
    const qc = useQueryClient()

    if (!project || !system || !model) {
        return <></>
    }

    const nodeTypes = React.useMemo(
        () => ({
            Class: LegacyNode,
            default: LegacyNode,
        }),
        []
    )

    const [nodes, setNodes] = React.useState(mapNodes(props.nodes))
    const [connections, setConnections] = React.useState(
        mapConnections(props.connections)
    )
    const [baseURL] = useAtom(baseURLRef)

    const modelMutation = usePutModel(baseURL, project, system, model)

    React.useEffect(() => {
        setNodes(mapNodes(props.nodes))
    }, [props.nodes])

    React.useEffect(() => {
        setConnections(mapConnections(props.connections))
    }, [props.connections])

    const onNodesChange = React.useCallback(
        (changes: NodeChange[]) => {
            setNodes((nodes: any) => {
                return pushNodeChanges(
                    changes,
                    nodes,
                    modelMutation.mutateAsync,
                    qc
                )
            })
        },
        [setNodes]
    )

    const onEdgesChange = React.useCallback(
        (changes: EdgeChange[]) => {
            setConnections(
                (eds) => [] // pushEdgeChanges(URL, id!, eds, changes, queryClient)
            )
        },
        [setConnections]
    )

    const onConnect = React.useCallback(
        (connection: any) => {
            setConnections((con) => {
                return []
            })
        },
        [setConnections]
    )

    return (
        <WrapBox>
            <ReactFlow
                nodeTypes={nodeTypes}
                nodes={nodes}
                edges={connections}
                onNodesChange={onNodesChange}
                onEdgesChange={onEdgesChange}
                onConnect={onConnect}
                onInit={() => {}}
                connectionLineType={ConnectionLineType.Step}
                fitView
            >
                <Controls />
                <Background />
            </ReactFlow>
        </WrapBox>
    )
}

export default Flow
