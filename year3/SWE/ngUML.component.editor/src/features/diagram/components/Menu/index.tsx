import {
    Button,
    Callout,
    Collapse,
    FormGroup,
    HTMLSelect,
    OptionProps,
} from '@blueprintjs/core'
import { useQueryClient, useQuery} from '@tanstack/react-query'
import { useNewProject } from 'api/project'
import { useNewSystem } from '@nguml-editor/diagram/mutations'
import { Project } from 'features/browser/types'
import { useProjects } from 'features/diagram/queries'
import { modelAtom, projectAtom, systemAtom } from 'features/diagram/state'
import { baseURL as baseURLAtom } from 'hooks/primitiveState'
import { useAtom } from 'jotai'
import React from 'react'
import styled from 'styled-components'

type MenuProps = {}

const Box = styled(Callout)`
    display: flex;
    flex-direction: column;
    gap: .3em;
`

const InnerFormGroup = styled.div`
    display: flex;
    flex-direction: row;
    gap: 1em;
`

export const Menu: React.FC<MenuProps> = () => {
    const [project, setProject] = useAtom(projectAtom)
    const [system, setSystem] = useAtom(systemAtom)
    const [model, setModel] = useAtom(modelAtom)
    const [baseURL] = useAtom(baseURLAtom)
    const [open, setOpen] = React.useState(false)


    const projectQuery = useProjects(baseURL)

    const newProject = useNewProject(baseURL)
    const newSystem = useNewSystem(baseURL, project)
    
    const newModel = () => {

        fetch(`${baseURL}/model/project/${project}/system/${system}/component/add_model/`, {
            method: 'POST',
            headers: {
                'content-type': 'application/json;charset=UTF-8',
            },
            body: JSON.stringify(
                {
                    title: "newModel",
                    system: Number(system),
                    raw_text: "default raw_text",
                    type: "Component",
                    time: "now",
                }
            )
        })
    }

    const queryClient = useQueryClient()
    const modelQuery = useQuery<Project[], Error>([`${baseURL}/model/project/${project}/system/${system}/component/get_model/`,
        '',
        'get'
    ])
    
    const getSystems = (e: Project[]) => {
        let _projs = e.filter((e) => e.id == parseInt(project))
        if (_projs.length) {
            return _projs[0].systems.map((s: any) => {
                let props: OptionProps = {
                    value: s.id,
                    label: `${s.name} (${s.description})`,
                }
                return props
            })
        }
        return []
    }

    const getModels = (e: Project[]) => {
        let _projs = e.filter((e) => e.id == parseInt(project))
        if (_projs.length) {
            let _proj = _projs[0]
            let _systems = _proj.systems.filter(
                (e: any) => e.id == parseInt(system)
            )
            if (_systems.length) {
                let _models = _systems[0].diagrams
                if (_models) {
                    let _modelKeys = Object.keys(_models)
                    return _modelKeys.map((s: any) => {
                        let props: OptionProps = {
                            value: s,
                            label: s,
                        }
                        return props
                    })
                }
            }
        }
        return []
    }

    if (!projectQuery.isSuccess) {
        return (
            <Box>
                Retreiving projects, if this takes too long - are you sure you
                are running ngUML.backend?
            </Box>
        )
    }
    return (
        <div>
            <Button
                onClick={() => setOpen((open) => !open)}
                intent={open ? 'warning' : 'primary'}
            >
                {open ? 'Hide' : 'Show'} menu
            </Button>
            <Collapse isOpen={open}>
                <br/>
                <FormGroup label={'Project'} helperText={'Select a Project'}>
                    <InnerFormGroup>
                        <HTMLSelect
                            fill
                            value={project}
                            onBlur={(e) => setProject(e.target.value)}
                            onChange={(e) => setProject(e.target.value)}
                            options={[
                                {
                                    value: '',
                                    label: 'Select a project...',
                                    disabled: true,
                                },
                                ...projectQuery.data.map((e: any) => {
                                    let props: OptionProps = {
                                        value: e.id,
                                        label: `${e.name} (${e.description})`,
                                    }
                                    return props
                                }),
                            ]}
                        ></HTMLSelect>
                        <Button
                            intent="success"
                            onClick={() => {
                                newProject
                                    .mutateAsync({
                                        name: Date.now().toString(),
                                        description: Date.now().toString(),
                                        text: 'empty',
                                        version: '1.0.0',
                                    })
                                    .then(() => {
                                        queryClient.refetchQueries()
                                    })
                            }}
                        >
                            New&nbsp;project
                        </Button>
                    </InnerFormGroup>
                </FormGroup>
                {parseInt(project) ? (
                    <>
                        <FormGroup
                            label={'System'}
                            helperText={'Select a System'}
                        >
                            <InnerFormGroup>
                                <HTMLSelect
                                    fill
                                    value={system}
                                    onBlur={(e) => setSystem(e.target.value)}
                                    onChange={(e) => setSystem(e.target.value)}
                                    options={[
                                        {
                                            value: '',
                                            label: 'Select a system...',
                                            disabled: true,
                                        },
                                        ...getSystems(projectQuery.data),
                                    ]}
                                ></HTMLSelect>
                                <Button
                                    intent="success"
                                    onClick={() => {
                                        newSystem
                                            .mutateAsync({
                                                name: Date.now().toString(),
                                                description:
                                                    Date.now().toString(),
                                                application_generated: false,
                                                version: '1.0.0',
                                                project: project,
                                            })
                                            .then(() => {
                                                queryClient.refetchQueries()
                                            })
                                    }}
                                >
                                    New&nbsp;system
                                </Button>
                            </InnerFormGroup>
                        </FormGroup>
                        {parseInt(system) ? (
                            <>
                                {!modelQuery.isSuccess ? (
                                    <FormGroup>
                                        <InnerFormGroup>
                                            <Button
                                                intent="success"
                                                onClick={() => {
                                                    newModel()
                                                    queryClient.refetchQueries()
                                                }}
                                            >
                                                New&nbsp;model
                                            </Button>
                                        </InnerFormGroup>
                                    </FormGroup>
                                ) : (
                                    <></>
                                )}
                            </>
                        ) : (
                            <>
                            </>
                        )}
                    </>
                ) : (
                    <></>
                )}
            </Collapse>
        </div>
    )
}

export default Menu
