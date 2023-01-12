import { Project, System } from '@nguml-editor/browser/types'
import React from 'react'
import styled from 'styled-components'
import BrowserTable from '../BrowserTable'

type BrowserProps = {
    data: Project[] | System[]
    title: string
    description: string
}

export const Browser: React.FC<BrowserProps> = ({
    title,
    description,
    data,
}) => {
    return <BrowserTable title={title} description={description} data={data} />
}

export default Browser
