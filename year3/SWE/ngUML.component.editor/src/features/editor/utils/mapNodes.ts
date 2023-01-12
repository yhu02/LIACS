import { Node } from '../types'
import { applyNodeChanges, Node as RFNode, NodeChange } from 'reactflow'

export const mapNodes = (nodes: any) => {
    return nodes.map((e: Node) => {
        console.log(e)
        return <RFNode>{
            id: e.id.toString(),
            position: {
                x: e.position?.x ?? 0,
                y: e.position?.y ?? 0,
            },
            data: e,
            type: e.type,
        }
    })
}
