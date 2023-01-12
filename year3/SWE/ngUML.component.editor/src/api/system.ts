import { useMutation, useQuery } from '@tanstack/react-query'

type Model = {
    id: number
    system_id: number
    type: string
    title: string
    raw_text: string
    time: string
    model_changed: boolean
}

type System = {
    id: number
    project_id: number
    name: string
    description: string
    version: string
    application_generated: boolean
    time: string
    models_changed: boolean
    models: Model[]
}

export const useSystems = (baseURL: string, project: number) => {
    return useQuery<System[], Error>([`${baseURL}/project/${project}/system/`])
}

export const useSystem = (baseURL: string, project: number, system: number) => {
    return useQuery<System | undefined, Error>([
        `${baseURL}/model/project/${project}/system/${system}`,
    ])
}

type ISystem = {
    name: String
    entities: String[]
    types: String[]
    version: String
}

export const useNewSystem = (baseURL: string, project: number) => {
    return useMutation<System | undefined, Error, ISystem>([
        'POST',
        `${baseURL}/model/project/${project}/system/`,
    ])
}

export const editSystem = (
    baseURL: string,
    project: number,
    system: number,
    entities: string[],
    types: string[],
    version: string
) => {
    return useMutation<System | undefined>(
        [`${baseURL}/model/project/${project}/system/${system}/`],
        async () => {
            return undefined
        }
    )
}
