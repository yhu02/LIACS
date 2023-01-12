import React from 'react'
import EditorState from '../../hooks/editorState'
import EditorData from '../../hooks/editorData'
import DiagramNode from '../../diagram/DiagramNode'
import Draggable from 'react-draggable'
import './_node.css'

interface INode {
    node: string
}

const Node: React.FC<INode & ReactFlowProps> = ({node}) => {
    const {
        setPanning,
        setFocus,
        setRedraw
    } = React.useContext(EditorState)!

    const {
        nodes,
        moveNode,
        references, 
        addRef
    } = React.useContext(EditorData)!
    
    const nodeRef = React.useRef<HTMLDivElement>(null)

    const handleMove = (x: number, y: number) => {
        moveNode(
            node,
            x,
            y
        )
    }

    React.useEffect(() => {
        if (!references[node]) {
            addRef(
                node,
                nodeRef
            )
        }
        if (references[node] && !(references[node]).current) {
            addRef(
                node,
                nodeRef
            )
        }
    }, [nodeRef]);
    
    return (
        <Draggable 
            nodeRef={nodeRef}
            grid={[8, 8]}
            position={nodes[node].position}
            onStop={(event, {lastX, lastY}) => {
                handleMove(lastX, lastY);
                setTimeout(() => setFocus(node), 1);
            }}
            onStart={() => {
                setPanning(false);
                setFocus(undefined);
            }}
            onDrag={() => {
                setTimeout(() => setRedraw(new Date().getTime()), 15);
            }}
        >
            <div
                className="nguml-diagram-node"
                id={'nodeRef-' + node}
                onMouseEnter={() => setPanning(false)}
                onMouseOut={() => setPanning(true)}
                onClick={() => {
                    setFocus(node);
                }}
                tabIndex={0}
                ref={nodeRef}
            >
                <DiagramNode
                    node={nodes[node]}
                />
            </div>
        </Draggable>
    )
}

export default Node