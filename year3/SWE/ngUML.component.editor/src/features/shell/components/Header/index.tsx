import React from 'react'
import './style/header.css'
import {
    Header as CarbonHeader,
    HeaderName,
    HeaderNavigation,
    HeaderMenuItem,
    HeaderGlobalAction,
    HeaderGlobalBar,
} from '@carbon/react'
import { NavLink, useNavigate, useParams } from 'react-router-dom'
import { Settings } from 'lucide-react'

type HeaderProps = {
    // children?: React.ReactNode
}

export const Header: React.FC<HeaderProps> = ({}) => {
    const nav = useNavigate()

    return (
        <CarbonHeader aria-label="ngUML Editor">
            <HeaderName prefix="ngUML">Editor</HeaderName>
            <HeaderNavigation aria-label="ngUML [Editor]">
                <NavLink style={{ display: 'flex' }} to="/">
                    {({ isActive }) => (
                        <HeaderMenuItem isActive={isActive}>
                            Home
                        </HeaderMenuItem>
                    )}
                </NavLink>
                <NavLink style={{ display: 'flex' }} to="/models">
                    {({ isActive }) => (
                        <HeaderMenuItem isActive={isActive}>
                            Models
                        </HeaderMenuItem>
                    )}
                </NavLink>
                <NavLink style={{ display: 'flex' }} to="/editor">
                    {<HeaderMenuItem>Editor (legacy)</HeaderMenuItem>}
                </NavLink>
            </HeaderNavigation>
            <HeaderGlobalBar>
                <HeaderGlobalAction onClick={() => nav('/settings')}>
                    <Settings />
                </HeaderGlobalAction>
            </HeaderGlobalBar>
        </CarbonHeader>
    )
}
