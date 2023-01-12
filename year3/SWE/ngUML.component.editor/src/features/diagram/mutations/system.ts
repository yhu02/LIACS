import { useMutation, useQuery } from '@tanstack/react-query'
import { System, ISystem } from '@nguml-editor/diagram/types/system'

export const useNewSystem = (baseURL: String, project: string) => {
    return useMutation<System | undefined, Error, ISystem>([
        'POST',
        `${baseURL}/model/project/${project}/system/`,
    ])
}
