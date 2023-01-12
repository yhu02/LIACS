import { Connection } from '../types'
import { applyEdgeChanges, Edge, EdgeChange } from 'reactflow'

export const mapConnections = (connections: Connection[]) => {
    return connections.map((e: Connection) => {
        return <Edge>{
            id: e.id.toString(),
            source: e.from?.toString(),
            target: e.to?.toString(),
            data: e,
            type: 'straight',
            label: 'connection',
        }
    })
}
