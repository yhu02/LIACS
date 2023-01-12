import React from 'react'
import { calcArrow } from './calcArrow'

interface IArrow {
    fromRef: React.RefObject<HTMLElement>;
    toRef: React.RefObject<HTMLElement>;
    gridRef: React.RefObject<HTMLElement>;
    fromLabel?: string | React.ReactElement;
    toLabel?: string | React.ReactElement;
    label?: string | React.ReactElement;
    fromArrowHead?: React.ReactElement;
    toArrowHead?: React.ReactElement;
    offset: number;
}

const sideLabelStyle : React.CSSProperties = {
    fontSize: 10,
    height: '16px',
    verticalAlign: 'middle',
    lineHeight: '16px',
    textOverflow: 'ellipsis',
    whiteSpace: 'nowrap',
    fontWeight: 'bolder',
    overflow: 'hidden',
    background: 'white'
}

const middleLabelStyle : React.CSSProperties = {
    display: 'inline-block',
    verticalAlign: 'middle',
    backgroundColor: 'white',
    textOverflow: 'ellipsis',
    whiteSpace: 'nowrap',
    overflow: 'hidden'
}

const labelStyle : React.CSSProperties = {
    position: 'absolute',
    height: 16,
    overflow: 'hidden',
    transformOrigin: 'left',
    display: 'flex',
    justifyContent: 'space-between',
    alignContent: 'space-between',
}

const arrowHeadStyle: React.CSSProperties = {
    position: 'absolute',
    width: 16,
    height: 16,
    overflow: 'hidden',
}

const Arrow: React.FC<IArrow> = ({
    fromRef,
    toRef,
    gridRef,
    fromLabel,
    toLabel,
    label,
    fromArrowHead,
    toArrowHead,
    offset
}) => {
    if (
        !fromRef || 
        !toRef || 
        !gridRef ||
        !fromRef.current || 
        !toRef.current ||
        !gridRef.current
        ) {
        return (
            <>
            </>
        )
    } else {
        // if fromRef = join,fork,merge,decision use child
        let fromRect = null;
        let toRect = null;
        const substrings = ['join','fork','merge','decision'];
        if(new RegExp(substrings.join("|")).test(fromRef.current.children[0].className))//if the any of the substrings are part of the classname,use the child
            fromRect = fromRef.current.children[0].children[0].getBoundingClientRect()
        else
            fromRect = fromRef.current.getBoundingClientRect()
        if(new RegExp(substrings.join("|")).test(toRef.current.children[0].className))//if the any of the substrings are part of the classname,use the child
            toRect = toRef.current.children[0].children[0].getBoundingClientRect()
        else
            toRect = toRef.current.getBoundingClientRect()
        const gridRect = gridRef.current.getBoundingClientRect()
        
        const arrow = calcArrow({
            fromRect,
            toRect,
            gridRect,
            offset
        })

        const flip = () => {
            if (arrow.arrowDirection < -90 && arrow.arrowDirection > -270) {
                return {
                    transform: 'scaleX(-1) scaleY(-1)',
                }
            } else {
                return {}
            }
        }

        let pathD = ''
        pathD += ' M ' + arrow.from.x + ', ' + arrow.from.y
        pathD += ' L ' + arrow.to.x + ', ' + arrow.to.y
        return (
            <>
                <g>
                    <path
                        stroke={'black'}
                        d={pathD}
                        transform={
                            'translate('+ gridRect.x +', '+ gridRect.y +')'
                        }
                    />
                    <foreignObject
                        style={{
                            position: 'absolute',
                            zIndex: -2,
                            width: '100vw',
                            height: '100vh',
                        }}
                    >
                        <div style={{
                            ...labelStyle,
                            left: arrow.from.x + gridRect.x,
                            top: arrow.from.y + gridRect.y - 8,
                            width: arrow.length,
                            transform: 'rotate(' + arrow.arrowDirection + 'deg)',
                            paddingLeft: fromArrowHead ? 24 : 8, 
                            paddingRight: toArrowHead ? 24 : 8,
                        }}>
                        <div style={{
                                ...arrowHeadStyle,
                                left: 0,
                            }}>
                                {fromArrowHead}
                            </div>
                            <div style={{
                                ...arrowHeadStyle,
                                right: 0,
                            }}>
                                {toArrowHead}
                            </div>
                            <span style={{
                                ...sideLabelStyle,
                                ...flip(),
                            }}>{fromLabel ?? ''}</span>
                            <span style={{
                                ...middleLabelStyle,
                                ...flip(),
                            }}>{label ?? ''}</span>
                            <span style={{
                                ...sideLabelStyle,
                                ...flip(),
                            }}>{toLabel ?? ''}</span>
                        </div>
                    </foreignObject>
                </g>
            </>
        )
    }
}

export default Arrow
