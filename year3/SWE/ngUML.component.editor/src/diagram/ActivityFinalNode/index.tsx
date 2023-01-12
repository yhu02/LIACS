import { relative } from 'path'
import React from 'react'
import { ReactFlowProps } from 'reactflow';
import './_activityfinalnode.css'

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

interface IActivityFinalNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const ActivityFinal: React.FC<IActivityFinalNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? []

    return (
        <div className={'nguml-activity-final-node-wrapper'}>
        <div    
            style={{
                ...propStyle
            }}
            // className={propClassName}
            className={[propClassName, "nguml-activity-final-outer-node"].join(" ")}
        >
            <div className={"nguml-activity-final-inner-node"}></div>
        </div>
        </div>
    )
}

export default ActivityFinal