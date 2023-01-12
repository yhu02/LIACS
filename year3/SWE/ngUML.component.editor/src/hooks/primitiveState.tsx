import { atomWithStorage } from 'jotai/utils'

export const baseURL = atomWithStorage('baseURL', 'http://localhost:8000/api')

export default {
    baseURL,
}
