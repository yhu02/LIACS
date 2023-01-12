import { Model } from './model'

export type System = {
    id: number
    project_id: number
    name: string
    description: string
    version: string
    application_generated: boolean
    time: string
    models_changed: boolean
    diagrams: Map<string, boolean>
}

export type ISystem = {
    name: String
    entities: String[]
    types: String[]
    version: String
}
