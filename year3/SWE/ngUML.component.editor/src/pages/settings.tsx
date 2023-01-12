import { Header } from '@nguml-editor/shell'
import { Tile } from '@carbon/react'
import React from 'react'
import styled from 'styled-components'
import { useAtom } from 'jotai'
import PrimitiveState from 'hooks/primitiveState'
import { TextInput } from 'carbon-components-react'

const Inset = styled.div`
    padding: 1em;
    margin-top: 3rem;
`

export const Main: React.FC = () => {
    const [baseURL, setBaseURL] = useAtom(PrimitiveState.baseURL)

    return (
        <>
            <Header />
            <Inset>
                <Tile>
                    Settings
                    <br />
                    <br />
                    <TextInput
                        labelText="Base URL"
                        helperText="Base URL where you run your Django application (default: http://localhost:8000)"
                        value={baseURL}
                        onChange={(e) => setBaseURL(e.target.value)}
                        light={true}
                        id="baseURL"
                    />
                </Tile>
            </Inset>
        </>
    )
}

export default Main
