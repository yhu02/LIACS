import { QueryClient, useMutation } from '@tanstack/react-query'
import { applyNodeChanges, Node as RFNode, NodeChange } from 'reactflow'

export const usePutModel = (
    baseURL: string,
    project: string,
    system: string,
    model: string
) => {
    return useMutation<any | undefined, Error, any>([
        'PUT',
        `${baseURL}/model/project/${project}/system/${system}/${model}/`,
    ])
}

export const useAutoLayout = (
    baseURL: string,
    project: string,
    system: string,
    model: string
) => {
    return useMutation<any | undefined, Error, any>([
        'POST',
        `${baseURL}/model/project/${project}/system/${system}/${model}/layout/`,
    ])
}

export const pushNodeChanges = (
    changes: any,
    nodes: any,
    mutateAsync: any,
    qc?: QueryClient
) => {
    let write = applyNodeChanges(changes, nodes)

    changes.forEach((change: NodeChange) => {
        if (change.type === 'position' && !change.dragging) {
            const node = nodes.filter((e: any) => e.id === change.id)[0]
            if (node) {
                mutateAsync([
                    {
                        type: 'moved-classifier',
                        to: {
                            position: {
                                x: Math.floor(node.position.x),
                                y: Math.floor(node.position.y),
                            },
                        },
                        key: node.data.name,
                    },
                ]).then(() => {
                    if (qc) {
                        qc.refetchQueries()
                    }
                })
            }
        }
    })

    return write
}
