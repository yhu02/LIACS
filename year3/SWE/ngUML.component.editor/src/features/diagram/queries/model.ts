import { useQuery } from '@tanstack/react-query'

export const useModel = (
    baseURL: String,
    project: string,
    system: string,
    model: string
) => {
    return useQuery<any, Error>([
        `${baseURL}/model/project/${project}/system/${system}/${model}/model`,
    ])
}
