import React from 'react'
import {
    ClassNode,
    EnumNode,
    InitialNode,
    ActionNode,
    DecisionNode
} from '../'
import ForkNode from "../ForkNode";
import JoinNode from "../JoinNode";
import MergeNode from "../MergeNode";
import ActivityFinal from "../ActivityFinalNode";
import FlowFinal from "../FlowFinalNode";
import { ReactFlowProps } from 'reactflow';

interface IDiagramNode {
    node: any;
}

const DiagramNode: React.FC<IDiagramNode & ReactFlowProps> = ({node}) => {
    if (node.type === 'Class')
        return (
            <ClassNode node={node}/>
        )
    if (node.type === 'Enum')
        return (
            <EnumNode node={node}/>
        )
    if (node.type === 'Action')
        return (
            <ActionNode node={node}/>
        )
    if (node.type === 'Initial')
        return (
            <InitialNode node={node}/>
        )
    if (node.type === 'Decision')
        return (
            <DecisionNode node={node}/>
        )
    if (node.type === 'Merge')
        return (
            <MergeNode node={node}/>
        )
    if (node.type === 'Fork')
        return (
            <ForkNode node={node}/>
        )
    if (node.type === 'Join')
        return (
            <JoinNode node={node}/>
        )
    if (node.type === 'ActivityFinal')
        return (
            <ActivityFinal node={node}/>
        )
    if (node.type === 'FlowFinal')
        return (
            <FlowFinal node={node}/>
        )
    else
        return (
            <div>
                {node.type} not implemented
            </div>
        )
}

export default DiagramNode
