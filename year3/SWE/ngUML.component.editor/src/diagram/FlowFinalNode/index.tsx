import { relative } from 'path'
import React from 'react'
import { ReactFlowProps } from 'reactflow';
import './_flowfinalnode.css'

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

interface IFlowFinalNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const FlowFinal: React.FC<IFlowFinalNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-flow-final-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-flow-final-node"].join(" ")}
        >
        </div>
        </div>
    )
}

export default FlowFinal