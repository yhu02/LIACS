import './../style.css'
import './_systemcreator.css'

// @ts-ignore
import {
    AppConnectivity,
    Collaborate,
    FlowConnection,
    SkipBack,
    SkipForward,
} from '@carbon/icons-react'
import {
    Button,
    Checkbox,
    Column,
    Grid,
    Loading,
    Row,
    TextInput,
    Tile,
    Toggle,
} from 'carbon-components-react'
import { ExtractionData, SystemData } from './../../hooks/preprocessingState'
import React, { useState } from 'react'

import { useAtom } from 'jotai'
import { baseURL as baseURLhook } from 'hooks/primitiveState'
import locale from './../../locales'
import { useNavigate } from 'react-router-dom'
import { usePreprocessingContext } from './../Shell'
import { useNewSystem } from '../../api/system'

export const SystemCreator: React.FC = () => {
    let navigate = useNavigate()

    const [preprocessingState, setPreprocessingState] =
        usePreprocessingContext()
    const [baseURL] = useAtom(baseURLhook)
    const newSystem = useNewSystem(baseURL, preprocessingState?.project?.id!)

    const [systemData, setSystemData] = useState<SystemData>(
        preprocessingState['system'] || {
            id: null,
            name: null,
            entities: [],
            types: [],
        }
    )
    const [loading, setLoading] = useState<boolean>(false)
    const [bucketing, setBucketing] = useState<boolean>(false)

    const toggleUMLTypes = (type: string) => {
        let updatedTypes: string[] | null = systemData['types']

        if (updatedTypes) {
            if (updatedTypes.includes(type)) {
                // Remove type from array if it's there
                updatedTypes = updatedTypes.filter(
                    (activeType) => activeType !== type
                )
            } else {
                // Add the type to the array if it's not there
                updatedTypes.push(type)
            }
        } else {
            updatedTypes = [type]
        }

        // Update state
        setSystemData({
            ...systemData,
            ...{
                types: updatedTypes,
            },
        })
    }

    const toggleEntities = (entity: string) => {
        let updatedEntities: string[] = systemData['entities']

        if (updatedEntities) {
            if (updatedEntities.includes(entity)) {
                // Remove entity from array if it's there
                updatedEntities = updatedEntities.filter(
                    (activeEntity) => activeEntity !== entity
                )
            } else {
                // Add the entity to the array if it's not there
                updatedEntities.push(entity)
            }
        } else {
            updatedEntities = [entity]
        }

        // Update state
        setSystemData({
            ...systemData,
            ...{
                entities: updatedEntities,
            },
        })
    }
    const selectAllEntities = () => {
        let updatedEntities: string[] = systemData['entities']
        if (updatedEntities.length === 0) {
            preprocessingState?.project?.topics?.map((entity) =>
                toggleEntities(entity)
            )
        } else {
            updatedEntities = []
            setSystemData({
                ...systemData,
                ...{
                    entities: updatedEntities,
                },
            })
        }
    }

    const createSystem = async () => {
        // Turn loading on
        setLoading(true)

        if (systemData['name']) {
            newSystem.mutate(
                {
                    name: systemData['name'],
                    entities: systemData['entities'],
                    types: systemData['types'],
                    version: '1.0',
                },
                {}
            )
        }

        // // Set system data and preprocessing data
        // const updatedSystemData: SystemData = {
        //   ...systemData,
        //   ...{
        //     id: system.inserted_id,
        //   },
        // }

        // Create all the selected models
        // const modelCreationReqs = systemData.types.map((uml_type: string) => {
        //   return new Promise((resolve, reject) => {
        //     fetch(
        //       `http://localhost:8000/api/model/${uml_type}/${preprocessingState?.project?.id}/${system.inserted_id}`,
        //       {
        //         method: 'POST',
        //         headers: {
        //           'Content-Type': 'application/json',
        //         },
        //         body: JSON.stringify({
        //           title: `${systemData['name']} - ${uml_type} model`,
        //           text: bucketing
        //             ? system.buckets[uml_type].join(' ')
        //             : preprocessingState?.project?.requirements,
        //         }),
        //       }
        //     )
        //       .then((response) => response.json())
        //       .then((response) => {
        //         resolve(response)
        //       })
        //       .catch((error) => {
        //         reject(error)
        //       })
        //   })
        // })
        // const metadata = await Promise.all(modelCreationReqs)
        //
        // const extractionData: ExtractionData = Object.keys(system.buckets).reduce(
        //   function (obj: any, uml_type: string) {
        //     let modelExtraction: any = metadata[systemData.types.indexOf(uml_type)]
        //     obj[uml_type] = {
        //       text: system.buckets[uml_type].join(' '),
        //       metadata: modelExtraction.metadata,
        //       model_id: modelExtraction.inserted_id,
        //     }
        //     return obj
        //   },
        //   {}
        // )
        //
        // setSystemData(updatedSystemData)
        // setPreprocessingState({
        //   ...preprocessingState,
        //   ...{
        //     system: updatedSystemData,
        //     extraction: extractionData,
        //   },
        // })

        // Go to the next step
        navigate('/requirements-preprocessing/review')

        // Turn loading off
        setLoading(false)
    }

    return (
        <section>
            <Grid>
                <Row>
                    <Column>
                        <h1>{locale['preprocessing.systemcreator.title']}</h1>
                    </Column>
                </Row>
                <Row>
                    <Column lg={8} md={12}>
                        <p>{locale['preprocessing.systemcreator.desc']}</p>
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
                                        'preprocessing.systemcreator.form.name.label'
                                    ]
                                }
                                placeholder={
                                    locale[
                                        'preprocessing.systemcreator.form.name.placeholder'
                                    ]
                                }
                                value={systemData['name'] || ''}
                                onChange={(e) => {
                                    setSystemData({
                                        ...systemData,
                                        ...{
                                            name: e.target.value,
                                        },
                                    })
                                }}
                            />
                            <fieldset className="cds--fieldset">
                                <legend className="cds--label">
                                    <Toggle
                                        id={'bucketing'}
                                        onToggle={setBucketing}
                                        labelText="Use bucketizer to generate subset"
                                        labelA="Off"
                                        labelB="On"
                                    />
                                </legend>
                            </fieldset>
                            <fieldset
                                className="cds--fieldset"
                                disabled={!bucketing}
                            >
                                <legend className="cds--label">
                                    {
                                        locale[
                                            'preprocessing.systemcreator.form.entities.label'
                                        ]
                                    }
                                </legend>
                                <Checkbox
                                    id={'SelectAll'}
                                    labelText={'Select all entities'}
                                    onChange={selectAllEntities}
                                    checked={
                                        preprocessingState?.project?.topics
                                            ?.length ===
                                        systemData['entities']?.length
                                    }
                                    indeterminate={
                                        systemData['entities']?.length !== 0 &&
                                        preprocessingState?.project?.topics
                                            ?.length !==
                                            systemData['entities']?.length
                                    }
                                />

                                {preprocessingState?.project?.topics?.map(
                                    (entity: string, index: number) => {
                                        return (
                                            <Checkbox
                                                key={index}
                                                checked={
                                                    systemData &&
                                                    systemData[
                                                        'entities'
                                                    ].includes(entity)
                                                }
                                                onChange={(e) =>
                                                    toggleEntities(entity)
                                                }
                                                labelText={
                                                    (entity &&
                                                        entity[0].toUpperCase() +
                                                            entity.slice(1)) ||
                                                    ''
                                                }
                                                id={entity.replace(' ', '-')}
                                            />
                                        )
                                    }
                                )}
                            </fieldset>

                            <div id="TypeSelector">
                                <label>
                                    {
                                        locale[
                                            'preprocessing.systemcreator.form.umltype.label'
                                        ]
                                    }
                                </label>
                                <div id="TypeOptions">
                                    <Button
                                        kind={
                                            systemData &&
                                            systemData['types'].includes(
                                                'class'
                                            )
                                                ? 'primary'
                                                : 'tertiary'
                                        }
                                        onClick={() => toggleUMLTypes('class')}
                                        size="xl"
                                        renderIcon={AppConnectivity}
                                    >
                                        {
                                            locale[
                                                'preprocessing.systemcreator.form.umltype.class'
                                            ]
                                        }
                                    </Button>
                                    <Button
                                        kind={
                                            systemData &&
                                            systemData['types'].includes(
                                                'activity'
                                            )
                                                ? 'primary'
                                                : 'tertiary'
                                        }
                                        onClick={() =>
                                            toggleUMLTypes('activity')
                                        }
                                        size="xl"
                                        renderIcon={FlowConnection}
                                    >
                                        {
                                            locale[
                                                'preprocessing.systemcreator.form.umltype.activity'
                                            ]
                                        }
                                    </Button>
                                    <Button
                                        kind={
                                            systemData &&
                                            systemData['types'].includes(
                                                'usecase'
                                            )
                                                ? 'primary'
                                                : 'tertiary'
                                        }
                                        onClick={() =>
                                            toggleUMLTypes('usecase')
                                        }
                                        size="xl"
                                        renderIcon={Collaborate}
                                    >
                                        {
                                            locale[
                                                'preprocessing.systemcreator.form.umltype.usecase'
                                            ]
                                        }
                                    </Button>
                                </div>
                            </div>
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
                                        'preprocessing.systemcreator.navigation.previous'
                                    ]
                                }
                            </Button>
                            <Button
                                renderIcon={SkipForward}
                                onClick={createSystem}
                                disabled={
                                    !(
                                        systemData['name'] &&
                                        systemData['entities'] &&
                                        systemData['types'].length > 0
                                    )
                                }
                            >
                                {
                                    locale[
                                        'preprocessing.systemcreator.navigation.next'
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

export default SystemCreator
