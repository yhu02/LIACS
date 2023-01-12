import { Header } from '@nguml-editor/shell'
import { Tile } from '@carbon/react'
import React from 'react'
import styled from 'styled-components'

const Inset = styled.div`
    padding: 1em;
    margin-top: 3rem;
`

export const Main: React.FC = () => {
    return (
        <>
            <Header />
            <Inset>
                <Tile>
                    <h1>404 - Page not found</h1>
                </Tile>
            </Inset>
        </>
    )
}

export default Main
