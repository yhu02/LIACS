import { useMutation, useQuery } from '@tanstack/react-query'

type Project = {
    id: number
    name: string
    text: string
    description: string
    version: string
    time: string
}

export const useProjects = (baseURL: String) => {
    return useQuery<Project[], Error>([`${baseURL}/model/project`])
}

export const useProject = (baseURL: String, id: number) => {
    return useQuery<Project | undefined, Error>([
        `${baseURL}/model/project/${id}`
    ])
}

type IProject = {
    name: string
    text: string
    version: string
    description: string
}

export const useNewProject = (baseURL: String) => {
    return useMutation<Project | undefined, Error, IProject>([
        'POST',
        `${baseURL}/model/project/`,
    ])
}
