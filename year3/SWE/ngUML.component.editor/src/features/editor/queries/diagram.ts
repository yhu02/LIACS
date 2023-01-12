import { useQuery } from '@tanstack/react-query'

// TODO: UNTYPED, NEEDS TYPING!
// ALSO ON BACKEND SIDE. FOR SE PROJECTS,
// IT WOULD BE GREAT IF YOU WOULD DIVE
// INTO AN API DESIGN HOW TO GET DIAGRAM
// INFO FROM BACKEND TO FRONTEND WITH
// STRONG TYPING. I RECOMMEND LOOKING INTO
// GRAPHQL.
export const useDiagram = (
    baseURL: String,
    project: String,
    system: String,
    model: String
) => {
    return useQuery<any, Error>([
        `${baseURL}/model/project/${project}/system/${system}/${model}/`,
    ])
}
