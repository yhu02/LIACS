import { relative } from 'path'
import React from 'react'
import { ReactFlowProps } from 'reactflow';
import '../_fork_and_joinnode.css'

interface IProperty {
    name: string;
    type: string;
}

interface INode {
    name: string;
    type: string;
    position: {
        x: number,
        y: number
    };
    data: Record<
        string,
        any
    >;
}

interface IForkNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const ForkNode: React.FC<IForkNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-fork-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-fork-node"].join(" ")}
        >
        </div>
            {node.name}
        </div>
    )
}

export default ForkNode