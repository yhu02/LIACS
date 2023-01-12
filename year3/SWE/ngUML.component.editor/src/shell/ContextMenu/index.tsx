import React from 'react'
import {
    HeaderPanel
} from 'carbon-components-react'
import EditorState from '../../hooks/editorState'
import EditorData from '../../hooks/editorData'
import { Menu as ClassMenu } from './ClassMenu/Menu'
import { Menu as EnumMenu } from './EnumMenu/Menu'
import { Menu as ActivityMenu } from './ActivityMenu/Menu'
import './_contextmenu.css'

const loadMenu: 
    (
        node?: any,
        focus?: string
    ) => 
    React.ReactElement = 
    (node,focus) => {
    if (!node || !focus) {
        return (
            <></>
        )
    } else if (node.type === 'Class') {
        return (
            <ClassMenu
                node={focus}
            />
        )
    } else if (node.type === 'Enum') {
        return (
            <EnumMenu
                node={focus}
            />
        )
    } else if (node.type === 'Initial') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'Action') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'Decision') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'Merge') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'Fork') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'Join') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'ActivityFinal') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'FlowFinal') {
        return (
            <ActivityMenu
                node={focus}
            />
        )
    } else if (node.type === 'handler') {
        return (
            <div></div>
        )
    
    } else {
        return (
            <div style={{color: 'red', textAlign: 'center', padding: 16}}>
                Context Menu not Implemented!
            </div>
        )
    }
}

export const ContextMenu: React.FC = () => {
    const {focus} = React.useContext(EditorState)!
    const {nodes} = React.useContext(EditorData)!
    
    return (
        <HeaderPanel
            aria-label="Context Menu"
            expanded={
                focus ? true : false
            }
            style={{
                overflowY: 'auto'
            }}
        >
            {
                loadMenu(nodes[focus!], focus)
            }
        </HeaderPanel>
    )
}

export default ContextMenu