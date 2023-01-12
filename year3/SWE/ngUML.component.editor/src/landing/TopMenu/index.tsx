import {
    Header,
    HeaderGlobalAction,
    HeaderGlobalBar,
    HeaderMenuItem,
    HeaderName,
    HeaderNavigation,
} from 'carbon-components-react'

import React from 'react'
import { Settings } from 'react-feather'
import { useNavigate } from 'react-router-dom'

export const TopMenu: React.FC = () => {
    const nav = useNavigate()

    return (
        <Header aria-label="ngUML User portal">
            <HeaderName prefix="ngUML">User portal</HeaderName>
            <HeaderNavigation aria-label="ngUML [Editor]">
                <HeaderMenuItem href="/">Home</HeaderMenuItem>
                <HeaderMenuItem href="/models">Models</HeaderMenuItem>
                <HeaderMenuItem href="/editor">Editor (legacy)</HeaderMenuItem>
            </HeaderNavigation>
            <HeaderGlobalBar>
                <HeaderGlobalAction onClick={() => nav('/settings')}>
                    <Settings />
                </HeaderGlobalAction>
            </HeaderGlobalBar>
        </Header>
    )
}

export default TopMenu
