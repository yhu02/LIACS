import React from 'react'
import { ReactFlowProps } from 'reactflow';
import {unEscapeCharacters,escapeCharacters,nl2br} from '../../commonFunctions'

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

interface IActionNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const actionWrapper: React.CSSProperties = {
    fontFamily: 'monospace',
    border: 'solid',
    borderColor: 'black',
    borderWidth: 1,
    display: 'block',
    backgroundColor: 'white',
    borderRadius: '10px',
    minHeight :50,
    lineHeight:"50px"
}

const actionName: React.CSSProperties = {
    borderColor: 'black',
    textAlign: 'center',
    borderWidth: 1,
    paddingTop: 8,
    paddingLeft: 16,
    paddingRight: 16,
    paddingBottom: 8,
    margin:'0 auto',
    verticalAlign:'middle',
    lineHeight: '15px',
}

const ActionNode: React.FC<IActionNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {

    const nodeName:string = nl2br(unEscapeCharacters(node.name));
    return (
        <div    
            style={{
                ...actionWrapper,
                ...propStyle
            }}
            className={propClassName}
        >
            <div style={actionName} dangerouslySetInnerHTML={{
                __html: nodeName
            }}>
            </div>
        </div>
    )
}

export default ActionNode