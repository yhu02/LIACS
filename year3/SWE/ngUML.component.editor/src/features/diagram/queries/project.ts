import { useMutation, useQuery } from '@tanstack/react-query'
import { Project, IProject } from '@nguml-editor/browser/types/project'

export const useProjects = (baseURL: String) => {
    return useQuery<Project[], Error>([`${baseURL}/model/project`,'','get'])
}

export const useProject = (baseURL: String, id: number) => {
    return useQuery<Project | undefined, Error>([
        `${baseURL}/model/project/${id}`,
    ])
}
