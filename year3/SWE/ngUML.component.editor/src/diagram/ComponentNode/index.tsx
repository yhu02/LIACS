import React from 'react'

interface ISubcomponent {
    name: string;
}

interface INode {
    name: string
    type: string
    position: {
        x: number,
        y: number
    }
    data: Record<string, any>
}

interface IcomponentNode {
    node: INode
    style?: React.CSSProperties
    className?: string
}

const classWrapper: React.CSSProperties = {
    fontFamily: 'monospace',
    border: 'solid',
    borderColor: 'black',
    borderWidth: 1,
    display: 'block',
    backgroundColor: 'white',
}

const className: React.CSSProperties = {
    borderBottom: 'solid',
    borderColor: 'black',
    textAlign: 'center',
    borderWidth: 1,
    paddingTop: 8,
    paddingLeft: 16,
    paddingRight: 16,
    paddingBottom: 8,
}

const classProperties: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    borderBottom: 'solid',
    borderColor: 'black',
    borderWidth: 1,
    paddingTop: 40,
    paddingLeft: 40,
    paddingRight: 40,
    paddingBottom: 40,
    textAlign: 'left',
    lineHeight: 5,
}

const ComponentNode: React.FC<IcomponentNode> = ({
    node,
    style: propStyle,
    className: subcomponentName,
}) => {
    const subcomponents: ISubcomponent[] = node['data']['subcomponents'] ?? []
    return (

        <div
            style={{
                ...classWrapper,
                ...propStyle,
            }}
            className={subcomponentName}
        >
            <div style={className}>{node.name}</div>
            <div style={classProperties}>
                {subcomponents.map((value: ISubcomponent, index: number) => {
                    if (value) {
                        return (
                            <span key={index} className="nguml-component-method">
                                {value.name}
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


export default ComponentNode
