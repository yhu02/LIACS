import { useLocation, useNavigate } from 'react-router-dom'

import Arrow from '../Arrow'
import EditorData from '../../hooks/editorData'
import EditorState from '../../hooks/editorState'
import React from 'react'

const groupedConnections = (connections: any) => {
    let obj: Record<string, any> = {}
    Object.keys(connections).forEach(
        (key: string) => {
            let nodes = [
                connections[key].from,
                connections[key].to
            ].sort()
            if (obj[nodes[0]]) {
                obj[nodes[0]].push(key)
            } else {
                obj[nodes[0]] = [key]
            }
        }
    )
    return obj
}

const ConnectionGrid = () => {
    const {
        connections,
        references,
        forceRefresh,
        currentActivity,
        importData,
    } = React.useContext(EditorData)!

    const {
        redraw
    } = React.useContext(EditorState)!

    const location = useLocation();
    const navigate = useNavigate()

    React.useEffect(() => {
        setRender(redraw + forceRefresh)
    }, [redraw, forceRefresh])

    React.useEffect(() => {        
        if (window) {
            const queryParams = new URLSearchParams(location.search);

            if (queryParams.has('from-preprocessing')) {
                // Load data
                importData(0, false);

                // Remove url parameter
                queryParams.delete('from-preprocessing')
                navigate(queryParams.toString(), { replace: true })
            }
        }
    }, [currentActivity, importData])
    
    const grouped = groupedConnections(connections)

    const [render, setRender] = React.useState<number>(0)

    return (
        <svg style={{
            zIndex: -2, 
            position: 'absolute', 
            width: '100vw', 
            height: '100vh'
        }}>
            <g>
                <text visibility={0}>{render}</text>
            </g>
        {
            Object.keys(grouped).map((keyName: string) => {
                return Object.keys(grouped[keyName]).map(
                    (connectionId: string, index: number) => {
                        let connection = connections[
                            grouped[keyName][index]
                        ]

                        let props = {
                            key: grouped[keyName][index],
                            fromRef: references[connection.from],
                            toRef: references[connection.to],
                            gridRef: references['grid'],
                            label: connection.label,
                            fromLabel: connection.labelFrom,
                            toLabel: connection.labelTo,
                            offset:
                                index % 2 ? 
                                - Math.ceil(index / 2) : 
                                Math.ceil(index / 2)
                        }

                        if (connection.type === 'generalization') {
                            return <Arrow
                                {...props}
                                fromLabel=''
                                toLabel=''
                                toArrowHead={
                                    <svg width={16} height={16} viewBox='0 0 16 16'>
                                        <path
                                            d="M 1, 16 L 1, 1 L 16, 8 L 1, 16 Z"
                                            stroke="black"
                                            fill="white"
                                        />
                                    </svg>
                                }
                            />
                        } else if (connection.type === 'association') {
                            return <Arrow
                                {...props}
                            />
                        } else if (connection.type === 'composition') {
                            return <Arrow
                                {...props}
                                fromArrowHead={
                                    <svg width={16} height={16} viewBox='0 0 16 16'>
                                        <path
                                            d="M 1, 8 L 8, 1 L 16, 8 L 8, 16 Z"
                                            stroke="black"
                                            fill="black"
                                        />
                                    </svg>
                                }
                            />
                        } else if (connection.type === 'edge') {
                            return <Arrow
                                {...props}
                                fromLabel=''
                                toLabel=''
                                toArrowHead={
                                    <svg width={16} height={16} viewBox='0 0 16 16'>
                                        <path
                                            d="M 1, 16 L 16,8 L 1, 1"
                                            stroke="black"
                                            fill="none"
                                        />
                                    </svg>
                                }
                            />
                        } else {
                            return <Arrow
                                {...props}
                            />
                        }
                    }
                )
            })
        }
        </svg>
    )
}

export default ConnectionGrid
