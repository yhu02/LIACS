import { Breadcrumbs, Header } from '@nguml-editor/shell'
import React from 'react'
import styled from 'styled-components'
import { Browser } from '@nguml-editor/browser'
import { useAtom } from 'jotai'
import { baseURL } from 'hooks/primitiveState'
import { useSystems } from '@nguml-editor/browser/queries'
import { InlineLoading } from 'carbon-components-react'
import { useParams } from 'react-router-dom'

const Inset = styled.div`
    padding: 1em;
    margin-top: 3rem;
`

export const ProjectIndex: React.FC = () => {
    const [URL] = useAtom(baseURL)
    const { project } = useParams()
    const systems = project ? useSystems(URL, project) : null

    return (
        <>
            <Header />
            <Inset>
                <Breadcrumbs
                    breadcrumbs={[
                        { name: 'Models', path: '/models' },
                        { name: `Project (${project})`, path: `/${project}` },
                    ]}
                />
                {systems?.isSuccess ? (
                    <Browser
                        title="Selecteer systeem"
                        description="Selecteer een systeem"
                        data={systems.data}
                    />
                ) : (
                    <InlineLoading />
                )}
            </Inset>
        </>
    )
}

export default ProjectIndex
