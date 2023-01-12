import React from 'react'
import EditorData from '../../../hooks/editorData'
import EditorState from '../../../hooks/editorState'
import { 
    ButtonGroup,
    Button,
    Drawer,
    EditableText,
    Alert,
    Icon
} from '@blueprintjs/core'
import './_classmenu.css'


interface IMenu {
    node: string
}

export const Menu: React.FC<IMenu> = ({node}) => {
    const {
        nodes,
        importedNodes,
        setNodes,
        deleteNode,
        copyNode,
        references
    } = React.useContext(EditorData)!

    const {
        setFocus
    } = React.useContext(EditorState)!

    const isImported = React.useMemo(() => {
        return Object.keys(importedNodes).indexOf(node) >= 0
    }, [node, importedNodes])

    const [delPrompt, setDelPrompt] = React.useState(false)

    document.onkeypress = function(evt) {
        evt = evt || window.event;
        const charCode = evt.keyCode || evt.which;
        if(charCode == 127){
            setDelPrompt(true);
        }
    };

    return (
        <>
            {
                isImported ?
                <div style={{
                    width: '100%',
                    padding: '8px',
                    textAlign: 'center'
                }}>
                    <Icon
                        icon='lock'
                        iconSize={32}
                    />
                </div>
                :
                <></>
            }
            <div 
                className="bp3-dark nguml-context-menu" 
                style={{
                    display: 'flex',
                    flexDirection: 'column',
                    alignItems: 'center',
                    justifyContent: 'center'
                }}
            >
                <Alert
                    className='bp3-dark'
                    intent='danger'
                    confirmButtonText='Yes'
                    icon='delete'
                    cancelButtonText='No'
                    isOpen={delPrompt}
                    onClose={() => setDelPrompt(false)}
                    onConfirm={() => {
                        setFocus(undefined);
                        deleteNode(node);
                    }}
                >
                    Are you sure you want to delete the node,
                    this is an irreversible action.
                </Alert>
                <ButtonGroup large fill>
                    <Button 
                        icon="duplicate"
                        onClick={() => {
                            copyNode(node);
                        }}
                    />
                    <Button 
                        icon="trash"
                        onClick={() => setDelPrompt(true)}
                        disabled={
                            isImported
                        }
                    />
                </ButtonGroup>
                <EditableText
                    value={nodes[node].name}
                    onChange={(e) => {
                        setNodes({
                            ...nodes,
                            [node]: {
                                ...nodes[node],
                                name: e
                            }
                        });
                    }}
                    maxLength={64}
                    className='nguml-class-menu-title'
                    disabled={isImported}
                />
            </div>
        </>
    )
}

export default Menu
