import React from 'react'
import { ReactFlowProps } from 'reactflow';

interface IEnumNode {
    node: any;
}

const EnumNode: React.FC<IEnumNode & ReactFlowProps> = ({node}) => {
    return (
        <div>
            {node.name}
        </div>
    )
}

export default EnumNode
