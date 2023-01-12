import { useMutation, useQuery } from '@tanstack/react-query'
import { Project, IProject } from '@nguml-editor/browser/types/project'

export const useNewProject = (baseURL: String) => {
    return useMutation<Project | undefined, Error, IProject>([
        'POST',
        `${baseURL}/model/project/`,
    ])
}
