import { useMutation, useQuery } from '@tanstack/react-query'
import { System, ISystem } from '@nguml-editor/browser/types/system'

export const useSystems = (baseURL: string, project: string) => {
    return useQuery<System[], Error>([
        `${baseURL}/model/project/${project}/system`,
    ])
}

export const useSystem = (baseURL: string, project: string, system: string) => {
    return useQuery<System | undefined, Error>([
        `${baseURL}/model/project/${project}/system/${system}`,
    ])
}

export const useNewSystem = (baseURL: string, project: string) => {
    return useMutation<System | undefined, Error, ISystem>([
        'POST',
        `${baseURL}/model/project/${project}/system/${project}`,
    ])
}

export const editSystem = (
    baseURL: string,
    project: string,
    system: string,
    entities: string[],
    types: string[],
    version: string
) => {
    return useMutation<System | undefined>(
        [`${baseURL}/model/project/${project}/system/${system}`],
        async () => {
            return undefined
        }
    )
}
