import { relative } from 'path'
import React from 'react'
import '../_decision_and_mergenode.css'
import {unEscapeCharacters,escapeCharacters,nl2br} from '../../commonFunctions'
import { ReactFlowProps } from 'reactflow';

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

interface IDecisionNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const DecisionNode: React.FC<IDecisionNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-decision-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-decision-node"].join(" ")}
        >
        </div>
            {unEscapeCharacters(node.name)}
        </div>
    )
}

export default DecisionNode