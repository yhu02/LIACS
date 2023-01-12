import React from 'react'
import Node from '../Node'
import EditorData from '../../hooks/editorData'
import { Loading } from 'carbon-components-react'

const NodeGrid: React.FC = () => {
    const {
        nodes,
        references,
        layout,
        addRef
    } = React.useContext(
        EditorData
    )!

    const gridRef = React.useRef<HTMLDivElement>(null)

    React.useEffect(() => {
        if (!references['grid']) {
            addRef('grid', gridRef)
        } else if (!references['grid'].current) {
            addRef('grid', gridRef)
        }
    })

    return (
        <div ref={gridRef}>
            {
                Object.keys(nodes).map((keyName: string, index: number) => {
                    return (
                        <Node 
                            key={keyName}
                            node={keyName}
                        />
                    )
                })
            }   
        </div>
    )
}

export default NodeGrid
