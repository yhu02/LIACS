import { System } from './system'

export type Project = {
    id: number
    name: string
    text: string
    description: string
    version: string
    time: string
    systems: System[]
}

export type IProject = {
    name: string
    text: string
    version: string
    description: string
}
