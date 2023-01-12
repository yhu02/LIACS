import React from 'react'
import { ReactFlowProps } from 'reactflow';

interface IProperty {
    name: string;
    type: string;
}

interface IMethod {
    name: string;
    type: string;
    code: string;
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

interface IClassNode {
    node: INode;
    style?: React.CSSProperties;
    className?: string;
}

const classWrapper: React.CSSProperties = {
    fontFamily: 'monospace',
    border: 'solid',
    borderColor: 'black',
    borderWidth: 1,
    display: 'block',
    backgroundColor: 'white'
}

const className: React.CSSProperties = {
    borderBottom: 'solid',
    borderColor: 'black',
    textAlign: 'center',
    borderWidth: 1,
    paddingTop: 8,
    paddingLeft: 16,
    paddingRight: 16,
    paddingBottom: 8
}

const classProperties: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    borderBottom: 'solid',
    borderColor: 'black',
    borderWidth: 1,
    paddingTop: 8,
    paddingLeft: 8,
    paddingRight: 8,
    paddingBottom: 8,
    textAlign: 'left',
    lineHeight: 1.5
}

const ClassNode: React.FC<IClassNode & ReactFlowProps> = ({
    node,
    style: propStyle,
    className: propClassName,
}) => {
    const properties : IProperty[] = node['data']['properties'] ?? [] 
    const methods : IMethod[] = node['data']['methods'] ?? []

    return (
        <div 
            style={{
                ...classWrapper,
                ...propStyle
            }}
            className={propClassName}
        >
            <div style={className}>
                {node.name}
            </div>
            <div style={classProperties}>
                {properties.map((value: IProperty, index: number) => {
                    if (value) {
                        return (
                            <span key={index} className="nguml-class-property">
                                +{value.name} : {value.type}
                            </span>
                        )
                    } else {
                        return <></>
                    }
                })}
            </div>
            <div style={classProperties}>
                {methods.map((value: IMethod, index: number) => {
                    if (value) {
                        return (
                            <span key={index} className="nguml-class-method">
                                {value.name}() : {value.type}
                            </span>
                        )
                    } else {
                        return <></>
                    }
                })}
            </div>
        </div>
    )
}

export default ClassNode
