import { Breadcrumbs, Header } from '@nguml-editor/shell'
import React from 'react'
import styled from 'styled-components'
import { Browser } from '@nguml-editor/browser'
import { useAtom } from 'jotai'
import { baseURL } from 'hooks/primitiveState'
import { useProjects } from '@nguml-editor/browser/queries'
import { InlineLoading } from 'carbon-components-react'

const Inset = styled.div`
    padding: 1em;
    margin-top: 3rem;
`

export const Models: React.FC = () => {
    const [URL] = useAtom(baseURL)
    const projects = useProjects(URL)

    return (
        <>
            <Header />
            <Inset>
                <Breadcrumbs
                    breadcrumbs={[{ name: 'Models', path: '/models' }]}
                />
                {projects.isSuccess ? (
                    <Browser
                        title={'Selecteer project'}
                        description={'Selecteer een project om verder te gaan'}
                        data={projects.data}
                    />
                ) : (
                    <InlineLoading />
                )}
            </Inset>
        </>
    )
}

export default Models
