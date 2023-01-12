import { Breadcrumbs, Header } from '@nguml-editor/shell'
import React from 'react'
import styled from 'styled-components'
import { Editor } from '@nguml-editor/editor'
import { useAtom } from 'jotai'
import { baseURL } from 'hooks/primitiveState'
import { useParams } from 'react-router-dom'

export const EditorIndex: React.FC = () => {
    const [URL] = useAtom(baseURL)
    const { project, system, model } = useParams()

    return project && system && model ? (
        <Editor baseURL={URL} project={project} system={system} model={model} />
    ) : (
        <span>Error.</span>
    )
}

export default EditorIndex
