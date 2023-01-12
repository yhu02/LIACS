import React from 'react'
import {
    Header as CarbonHeader,
    HeaderName,
    HeaderNavigation,
    HeaderMenuItem,
    HeaderGlobalAction,
    HeaderGlobalBar,
} from '@carbon/react'
import { NavLink, useNavigate, useParams } from 'react-router-dom'
import { Settings, X } from 'lucide-react'
import { XCircle } from 'react-feather'
import { Icon } from '@blueprintjs/core'
import { useAutoLayout } from 'features/editor/mutations/model'
import { useAtom } from 'jotai'
import { baseURL as baseURLhook } from 'hooks/primitiveState'
import { useQueryClient } from '@tanstack/react-query'

type HeaderProps = {
    // children?: React.ReactNode
}

export const Header: React.FC<HeaderProps> = ({}) => {
    const nav = useNavigate()
    const { project, system, model } = useParams()

    if (!project || !system || !model) {
        return <></>
    }

    const [baseURL] = useAtom(baseURLhook)
    const autoLayout = useAutoLayout(baseURL, project, system, model)
    const qc = useQueryClient()

    return (
        <CarbonHeader aria-label="ngUML Editor">
            <HeaderName prefix="ngUML">Editor</HeaderName>
            <HeaderNavigation aria-label="ngUML [Editor]">
                <HeaderMenuItem>Up-to-date</HeaderMenuItem>
            </HeaderNavigation>
            <HeaderGlobalBar>
                {model == 'class' && (
                    <HeaderGlobalAction
                        aria-label="Layout"
                        onClick={() =>
                            autoLayout
                                .mutateAsync({})
                                .then(() => qc.refetchQueries())
                        }
                    >
                        <Icon icon="graph" />
                    </HeaderGlobalAction>
                )}
                <HeaderGlobalAction
                    onClick={() => nav(`/${project}/${system}`)}
                >
                    <XCircle />
                </HeaderGlobalAction>
            </HeaderGlobalBar>
        </CarbonHeader>
    )
}
