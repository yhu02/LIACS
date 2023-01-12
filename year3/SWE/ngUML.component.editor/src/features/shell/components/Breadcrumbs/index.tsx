import { Breadcrumb, BreadcrumbItem } from '@carbon/react'
import React from 'react'
import { Link } from 'react-router-dom'

type Breadcrumb = {
    name: string
    path?: string
}

type BreadcrumbsProps = {
    breadcrumbs: Breadcrumb[]
}

export const Breadcrumbs: React.FC<BreadcrumbsProps> = ({ breadcrumbs }) => {
    return (
        <div style={{ padding: '1rem' }}>
            <Breadcrumb noTrailingSlash>
                {breadcrumbs.map((e) => (
                    <BreadcrumbItem>
                        {e.path ? <Link to={e.path}>{e.name}</Link> : <></>}
                    </BreadcrumbItem>
                ))}
            </Breadcrumb>
        </div>
    )
}
