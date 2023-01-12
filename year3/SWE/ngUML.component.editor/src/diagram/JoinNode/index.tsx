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

interface IJoinNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const JoinNode: React.FC<IJoinNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-join-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-join-node"].join(" ")}
        >
        </div>
            {node.name}
        </div>
    )
}

export default JoinNode