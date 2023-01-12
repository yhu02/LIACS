import React from 'react'
import { NodeGrid, ConnectionGrid } from '../../editor'
import EditorState from '../../hooks/editorState'
import EditorData from '../../hooks/editorData'
import { TransformComponent, TransformWrapper } from 'react-zoom-pan-pinch'
import { Loading } from 'carbon-components-react'
import './_canvas.css'

const Canvas: React.FC = () => {
    const {
        panning,
        setFocus,
        setRedraw
    } = React.useContext(EditorState)!

    const {
        layout
    } = React.useContext(EditorData)!

    return (
        <>
            {layout ? <Loading/> : <></>}
            <div 
                onClick={() => {
                    if (panning) {
                        setFocus(undefined)
                    }
                }}
                className="nguml-canvas"
            >
                <TransformWrapper
                    options={{
                        limitToBounds: false,
                        minScale: 1,
                        maxScale: 1
                    }}
                    pan={{
                        disabled: !panning
                    }}
                    onPanning={() => setRedraw(new Date().getTime())}
                >
                    <TransformComponent>
                        <div style={{
                            width: '100vw',
                            height: '100vh'
                        }}>
                            <NodeGrid/>
                        </div>
                    </TransformComponent>
                </TransformWrapper>
            </div>
            <ConnectionGrid/>
        </>
    )
}

export default Canvas
