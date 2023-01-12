import { useDiagram } from '@nguml-editor/editor/queries'
import { Flow, Header } from '@nguml-editor/editor/components'
import React from 'react'
import { mapNodes, mapConnections } from '@nguml-editor/editor/utils'
import { Loading } from 'carbon-components-react'

type EditorProps = {
    model: string
    project: string
    system: string
    baseURL: string
}

export const Editor: React.FC<EditorProps> = ({
    baseURL,
    project,
    system,
    model,
}) => {
    const diagram = useDiagram(baseURL, project, system, model)

    if (!diagram.isSuccess) {
        return (
            <>
                <Header />
                <Loading />
            </>
        )
    }
    const nodes = diagram.data.nodes
    const connections = diagram.data.connections

    return (
        <>
            <Header />
            <Flow nodes={nodes} connections={connections} />
        </>
    )
}

export default Editor
