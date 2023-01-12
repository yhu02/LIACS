import './_projectselector.css'

// @ts-ignore
import { Add, ArrowRight } from '@carbon/icons-react'
import {
    Button,
    Column,
    DataTable,
    DataTableRow,
    DataTableSkeleton,
    Grid,
    Row,
} from 'carbon-components-react'
import React, { useEffect, useState } from 'react'

import type { DataTableHeader } from 'carbon-components-react'
import { ProjectData } from './../../hooks/preprocessingState'
import locale from './../../locales'
import { useNavigate } from 'react-router-dom'
import { usePreprocessingContext } from './../Shell'
import { useProjects } from '../../api/project'
import { useAtom } from 'jotai'
import { baseURL as baseURLhook } from 'hooks/primitiveState'

const {
    TableContainer,
    Table,
    TableHead,
    TableRow,
    TableBody,
    TableCell,
    TableHeader,
    TableToolbar,
    TableToolbarContent,
    TableToolbarSearch,
} = DataTable

export const ProjectSelector: React.FC = () => {
    let navigate = useNavigate()

    const [preprocessingState, setPreprocessingState] =
        usePreprocessingContext()
    const [baseURL] = useAtom(baseURLhook)
    const { data } = useProjects(baseURL)
    const projects = data

    const headers: DataTableHeader<string>[] = [
        {
            key: 'name',
            header: locale[
                'preprocessing.projectselector.table.header.project'
            ],
        },
        {
            key: 'description',
            header: locale[
                'preprocessing.projectselector.table.header.description'
            ],
        },
        {
            key: 'id',
            header: '',
        },
    ]

    const setProject = (id: number) => {
        // Get project with selected id
        const project =
            projects &&
            projects.filter((project) => project && project.id === id)[0]

        if (project) {
            setPreprocessingState((state: any) => {
                return {
                    ...state,
                    ...{
                        project: {
                            id: project.id,
                            name: project.name,
                            desc: project.description,
                            requirements: project.text,
                            topics: [],
                        },
                    },
                }
            })
        }

        // Continue the flow to the next part
        navigate('/requirements-preprocessing/system')
    }

    return (
        <section>
            <Grid>
                <Row>
                    <Column>
                        <h1>{locale['preprocessing.projectselector.title']}</h1>
                    </Column>
                </Row>
                <Row>
                    <Column lg={8} md={12}>
                        <p>{locale['preprocessing.projectselector.desc']}</p>
                    </Column>
                </Row>
                <Row>
                    <Column className="PreprocessingContentTile">
                        {projects ? (
                            <>
                                <DataTable
                                    rows={projects as any}
                                    headers={headers}
                                    render={({
                                        rows,
                                        headers,
                                        onInputChange,
                                        getHeaderProps,
                                        getBatchActionProps,
                                    }) => (
                                        <TableContainer
                                            title={
                                                locale[
                                                    'preprocessing.projectselector.table.title'
                                                ]
                                            }
                                            description={
                                                locale[
                                                    'preprocessing.projectselector.table.desc'
                                                ]
                                            }
                                        >
                                            <TableToolbar>
                                                <TableToolbarContent>
                                                    <TableToolbarSearch
                                                        tabIndex={
                                                            getBatchActionProps()
                                                                .shouldShowBatchActions
                                                                ? -1
                                                                : 0
                                                        }
                                                        onChange={onInputChange}
                                                    />
                                                    <Button
                                                        tabIndex={
                                                            getBatchActionProps()
                                                                .shouldShowBatchActions
                                                                ? -1
                                                                : 0
                                                        }
                                                        onClick={() =>
                                                            navigate(
                                                                '/requirements-preprocessing/project/create'
                                                            )
                                                        }
                                                        size="small"
                                                        kind="primary"
                                                        renderIcon={Add}
                                                    >
                                                        {
                                                            locale[
                                                                'preprocessing.projectselector.table.actions.add'
                                                            ]
                                                        }
                                                    </Button>
                                                </TableToolbarContent>
                                            </TableToolbar>
                                            <Table>
                                                <TableHead>
                                                    <TableRow>
                                                        {headers.map(
                                                            (header) => (
                                                                <TableHeader
                                                                    {...getHeaderProps(
                                                                        {
                                                                            header,
                                                                        }
                                                                    )}
                                                                >
                                                                    {
                                                                        header.header
                                                                    }
                                                                </TableHeader>
                                                            )
                                                        )}
                                                    </TableRow>
                                                </TableHead>
                                                <TableBody>
                                                    {rows.map((row) => (
                                                        <TableRow key={row.id}>
                                                            {row.cells.map(
                                                                (cell) => {
                                                                    if (
                                                                        cell.id.split(
                                                                            ':'
                                                                        )[1] ===
                                                                        'id'
                                                                    ) {
                                                                        return (
                                                                            <TableCell
                                                                                key={
                                                                                    cell.id
                                                                                }
                                                                                align="right"
                                                                            >
                                                                                <Button
                                                                                    onClick={() =>
                                                                                        setProject(
                                                                                            cell.value
                                                                                        )
                                                                                    }
                                                                                    hasIconOnly={
                                                                                        true
                                                                                    }
                                                                                    iconDescription={
                                                                                        locale[
                                                                                            'preprocessing.projectselector.table.body.btn.tooltip'
                                                                                        ]
                                                                                    }
                                                                                    tooltipPosition="left"
                                                                                >
                                                                                    <ArrowRight
                                                                                        size={
                                                                                            16
                                                                                        }
                                                                                    />
                                                                                </Button>
                                                                            </TableCell>
                                                                        )
                                                                    } else {
                                                                        return (
                                                                            <TableCell
                                                                                key={
                                                                                    cell.id
                                                                                }
                                                                            >
                                                                                {
                                                                                    cell.value
                                                                                }
                                                                            </TableCell>
                                                                        )
                                                                    }
                                                                }
                                                            )}
                                                        </TableRow>
                                                    ))}
                                                </TableBody>
                                            </Table>
                                        </TableContainer>
                                    )}
                                />
                                {projects.length === 0 && (
                                    <div id="EmptyDataTableRow">
                                        <p>
                                            {
                                                locale[
                                                    'preprocessing.projectselector.table.empty'
                                                ]
                                            }
                                        </p>
                                    </div>
                                )}
                            </>
                        ) : (
                            <DataTableSkeleton />
                        )}
                    </Column>
                </Row>
            </Grid>
        </section>
    )
}

export default ProjectSelector
