import { relative } from 'path'
import React from 'react'
import { ReactFlowProps } from 'reactflow';
import './_initialnode.css'

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

interface IInitialNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const InitialNode: React.FC<IInitialNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-initial-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-initial-node"].join(" ")}
        >
        </div>
        </div>
    )
}

export default InitialNode