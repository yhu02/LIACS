import { useMutation } from '@tanstack/react-query'

export const useNewModel = (
    baseURL: String,
    project: string,
    system: string,
    model: string
) => {
    return useMutation<any, Error, any>([
        'POST',
        `${baseURL}/model/project/${project}/system/${system}/${model}/model/`,
    ])
}
