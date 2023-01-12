import { Breadcrumbs, Header } from '@nguml-editor/shell'
import React from 'react'
import styled from 'styled-components'
import { useAtom } from 'jotai'
import { baseURL } from 'hooks/primitiveState'
import { useSystem } from '@nguml-editor/browser/queries'
import {
    Button,
    Tile,
    InlineLoading,
    TableContainer,
} from 'carbon-components-react'
import { useNavigate, useParams } from 'react-router-dom'
import { FileBox } from 'lucide-react'

const Inset = styled.div`
    padding: 1em;
    margin-top: 3rem;
`

export const SystemIndex: React.FC = () => {
    const [URL] = useAtom(baseURL)
    const { project, system } = useParams()
    const systemObj = project && system ? useSystem(URL, project, system) : null
    const nav = useNavigate()

    return (
        <>
            <Header />
            <Inset>
                <Breadcrumbs
                    breadcrumbs={[
                        { name: 'Models', path: '/models' },
                        { name: `Project (${project})`, path: `/${project}` },
                        {
                            name: `System (${system})`,
                            path: `/${project}/${system}`,
                        },
                    ]}
                />
                {systemObj?.isSuccess ? (
                    <TableContainer
                        title={'Selecteer een model'}
                        description={
                            'Selecteer een UML-model om mee aan de slag te gaan'
                        }
                    >
                        <Tile style={{ backgroundColor: '#cccccc' }}>
                            {Object.entries(systemObj.data?.diagrams ?? {}).map(
                                (val) => (
                                    <Button
                                        size="lg"
                                        style={{ margin: '1em' }}
                                        onClick={() => nav(val[0])}
                                    >
                                        <div
                                            style={{
                                                display: 'flex',
                                                flexDirection: 'column',
                                                justifyContent: 'space-between',
                                                gap: '.5em',
                                            }}
                                        >
                                            <span>
                                                {`UML ${
                                                    val[0]
                                                        .charAt(0)
                                                        .toUpperCase() +
                                                    val[0].slice(1)
                                                } Model`}
                                            </span>
                                            <FileBox
                                                style={{ alignSelf: 'start' }}
                                            />
                                        </div>
                                    </Button>
                                )
                            )}
                        </Tile>
                    </TableContainer>
                ) : (
                    <InlineLoading />
                )}
            </Inset>
        </>
    )
}

export default SystemIndex
