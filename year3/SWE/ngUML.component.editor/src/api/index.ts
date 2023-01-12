import { QueryClient } from '@tanstack/react-query'
import axios from 'axios'

async function mutationFn(this: any, variables: any) {
    if (
        typeof this == 'object' &&
        Array.isArray(this.mutationKey) &&
        this.mutationKey.length == 2
    ) {
        if (['POST', 'PUT', 'PATCH'].includes(this.mutationKey[0])) {
            if (this.mutationKey[1]) {
                try {
                    switch (this.mutationKey[0]) {
                        case 'POST':
                            let rPOST = await axios.post(
                                `${this.mutationKey[1]}`,
                                variables
                            )
                            return rPOST.data
                        case 'PUT':
                            let rPUT = await axios.put(
                                `${this.mutationKey[1]}`,
                                variables
                            )
                            return rPUT.data
                        case 'PATCH':
                            let rPATCH = await axios.patch(
                                `${this.mutationKey[1]}`,
                                variables
                            )
                            return rPATCH.data
                        default:
                            return {}
                    }
                } catch (exception) {
                    console.error(`
    ⚠️ mutationFn failed to complete properly!
  `)
                    console.error(exception)
                }
            }
        } else {
            console.error(
                '⚠️ mutationFn only accepts POST, PUT or PATCH, provide it in the mutationKey!'
            )
        }
    } else {
        console.error(`
    ⚠️ mutationFn was invoked without or with a malfunctioned a mutationKey. 
    Please provide a mutationKey (['$METHOD', '$ENDPOINT'] e.g. ['POST', '/model/projects'])
    to do a mutation.
  `)
    }
}


export const queryClient = new QueryClient({
    defaultOptions: {
        queries: {
            queryFn: async ({ queryKey }) => {
                const { data } = await axios({
                    url: `${queryKey[0]}`,
                    method: `get`,
                })
                return data
            },
        },
        mutations: {
            mutationFn: mutationFn,
        },
    },
})