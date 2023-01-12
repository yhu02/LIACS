import './../style.css'
import './_projectcreator.css'

import {
    Button,
    Column,
    FileUploaderDropContainer,
    Grid,
    Loading,
    Row,
    Tag,
    TextArea,
    TextInput,
    Tile,
} from 'carbon-components-react'
import React, { useState } from 'react'
// @ts-ignore
import { SkipBack, SkipForward } from '@carbon/icons-react'

import { ProjectData } from './../../hooks/preprocessingState'
import locale from './../../locales'
import { useNavigate } from 'react-router-dom'
import { usePreprocessingContext } from './../Shell'
import { useNewProject } from '../../api/project'
import { useAtom } from 'jotai'
import { baseURL as baseURLhook } from 'hooks/primitiveState'

export const ProjectCreator: React.FC = () => {
    let navigate = useNavigate()

    const [preprocessingState, setPreprocessingState] =
        usePreprocessingContext()
    const [projectData, setProjectData] = useState<ProjectData>(
        preprocessingState['project'] || {
            id: null,
            name: null,
            desc: null,
            requirements: null,
            topics: null,
        }
    )
    const [baseURL] = useAtom(baseURLhook)
    const newProject = useNewProject(baseURL)
    const [loading, setLoading] = useState<boolean>(false)

    const createProject = async () => {
        // Turn loading on
        setLoading(true)

        if (
            projectData['name'] &&
            projectData['requirements'] &&
            projectData['desc']
        ) {
            newProject.mutate(
                {
                    name: projectData['name'],
                    text: projectData['requirements'],
                    description: projectData['desc'],
                    version: '1.0',
                },
                {}
            )
        }

        // // Set project data and preprocessing data
        // const updatedProjectData: ProjectData = {
        //   ...projectData,
        //   ...{
        //     id: project.inserted_id,
        //     topics: project.available_topics,
        //   },
        // }
        // setProjectData(updatedProjectData)
        // setPreprocessingState({
        //   ...preprocessingState,
        //   ...{
        //     project: updatedProjectData,
        //   },
        // })

        // Go to the next step
        navigate('/requirements-preprocessing/system')

        // Turn loading off
        setLoading(false)
    }

    return (
        <section>
            <Grid>
                <Row>
                    <Column>
                        <h1>{locale['preprocessing.projectcreator.title']}</h1>
                    </Column>
                </Row>
                <Row>
                    <Column lg={8} md={12}>
                        <p>{locale['preprocessing.projectcreator.desc']}</p>
                    </Column>
                </Row>
                <Row>
                    <Column>
                        <Tile className="mt">
                            <TextInput
                                className="mb"
                                id="name"
                                labelText={
                                    locale[
                                        'preprocessing.projectcreator.form.name.label'
                                    ]
                                }
                                placeholder={
                                    locale[
                                        'preprocessing.projectcreator.form.name.placeholder'
                                    ]
                                }
                                value={projectData['name'] || ''}
                                onChange={(e) => {
                                    setProjectData({
                                        ...projectData,
                                        ...{
                                            name: e.target.value,
                                        },
                                    })
                                }}
                            />
                            <TextArea
                                id="description"
                                labelText={
                                    locale[
                                        'preprocessing.projectcreator.form.description.label'
                                    ]
                                }
                                placeholder={
                                    locale[
                                        'preprocessing.projectcreator.form.description.placeholder'
                                    ]
                                }
                                rows={4}
                                value={projectData['desc'] || ''}
                                onChange={(e) => {
                                    setProjectData({
                                        ...projectData,
                                        ...{
                                            desc: e.target.value,
                                        },
                                    })
                                }}
                            />

                            <Row>
                                <Column lg={9} className="mt">
                                    <div className="RequirementInputContainer">
                                        <TextArea
                                            id="requirements"
                                            labelText={
                                                locale[
                                                    'preprocessing.projectcreator.form.requirements.label'
                                                ]
                                            }
                                            placeholder={
                                                locale[
                                                    'preprocessing.projectcreator.form.requirements.placeholder'
                                                ]
                                            }
                                            rows={20}
                                            value={
                                                projectData['requirements'] ||
                                                ''
                                            }
                                            onChange={(e) => {
                                                setProjectData({
                                                    ...projectData,
                                                    ...{
                                                        requirements:
                                                            e.target.value,
                                                    },
                                                })
                                            }}
                                        />
                                        <div className="OrContainer">
                                            <Tag
                                                type="cool-gray"
                                                title={locale[
                                                    'preprocessing.projectcreator.form.or'
                                                ].toUpperCase()}
                                            >
                                                {locale[
                                                    'preprocessing.projectcreator.form.or'
                                                ].toUpperCase()}
                                            </Tag>
                                            <hr />
                                        </div>
                                    </div>
                                </Column>
                                <Column lg={3} className="mt">
                                    <div className="cds--file__container">
                                        <strong className="cds--file--label">
                                            {
                                                locale[
                                                    'preprocessing.projectcreator.form.file.label'
                                                ]
                                            }
                                        </strong>
                                        <p className="cds--label-description">
                                            {
                                                locale[
                                                    'preprocessing.projectcreator.form.file.desc'
                                                ]
                                            }
                                        </p>
                                        <FileUploaderDropContainer
                                            accept={['text/plain', 'audio/wav']}
                                            labelText={
                                                locale[
                                                    'preprocessing.projectcreator.form.file.helper'
                                                ]
                                            }
                                            tabIndex={0}
                                        />
                                    </div>
                                </Column>
                            </Row>
                        </Tile>
                    </Column>
                </Row>
                <Row>
                    <Column>
                        <div className="FlowNavigation">
                            <Button
                                renderIcon={SkipBack}
                                kind="tertiary"
                                onClick={() => navigate(-1)}
                            >
                                {
                                    locale[
                                        'preprocessing.projectcreator.navigation.previous'
                                    ]
                                }
                            </Button>
                            <Button
                                renderIcon={SkipForward}
                                onClick={createProject}
                                disabled={
                                    !(
                                        projectData['name'] &&
                                        projectData['desc'] &&
                                        projectData['requirements']
                                    )
                                }
                            >
                                {
                                    locale[
                                        'preprocessing.projectcreator.navigation.next'
                                    ]
                                }
                            </Button>
                        </div>
                    </Column>
                </Row>
            </Grid>

            <Loading
                description="Loading..."
                withOverlay={true}
                active={loading}
            />
        </section>
    )
}

export default ProjectCreator
