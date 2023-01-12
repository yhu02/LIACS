import React from 'react'
import { DiagramNode } from 'diagram'
import { Handle, Position } from 'reactflow'

type LegacyNodeProps = {
    data: any
}

export const LegacyNode: React.FC<LegacyNodeProps> = ({ data }) => {
    return (
        <div style={{ display: 'flex' }}>
            <Handle type="target" position={Position.Top} />
            <DiagramNode node={data} />
            <Handle type="source" position={Position.Bottom} />
        </div>
    )
}

export default LegacyNode
