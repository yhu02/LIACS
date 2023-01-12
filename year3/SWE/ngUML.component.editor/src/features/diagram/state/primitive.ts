import { atom } from 'jotai'

export const projectAtom = atom<string>('')
export const systemAtom = atom<string>('')
export const modelAtom = atom<string>('')
export const focusTypeAtom = atom<string>('')
export const focusAtom = atom<string>('')
export const countAtom = atom<number>(0)
export const listAtom = atom<any>([
    "",
    0
])

export const relationsAtom = atom<any>([
    "",
    0
])
    
export interface nameandid {
    name: string
    id: number | null
}

export const nodeDataAtom = atom<any>({
    'model': {
        'id': 1,
        'system': 1,
        'type': 'Component',
        'title': 'titeltje',
        'raw_text': 'rauwe text enzo',
        'time': '2022-11-13T21:34:08.023628Z'
    },
    'nodes': [{
        'id': 1,
        'parent': null,
        'name': 'component1',
        'x': 1,
        'y': 2,
        'model': 1,
        'nodes': [{
            'id': 3,
            'parent': 1,
            'name': 'teeeest',
            'x': 70,
            'y': 2,
            'model': 1,
            'nodes': [{
                'id': 4,
                'parent': 3,
                'name': 'blabla',
                'x': 70,
                'y': 2,
                'model': 1,
                'nodes': []
            }]
        }]
    }, {
        'id': 2,
        'parent': null,
        'name': 'blabla',
        'x': 70,
        'y': 2,
        'model': 1,
        'nodes': []
    }]
})

export const localModelAtom = atom<localModel>({
    model: {
        id: 1,
        system: 1,
        type: "Component",
        title: "title111",
        raw_text: "A model 22222",
        time: "2022-11-13T21:34:08.023628Z"
    },
    nodes: [
        {
            id: 2,
            parent: null,
            name: "subffffffff",
            x: 8,
            y: 8,
            model: 1,
            nodes: [],
        },
        {
            id: 3,
            parent: null,
            name: "subnodeNameTwo",
            x: 8,
            y: 8,
            model: 1,
            nodes: [
                {
                    id: 14,
                    parent: 3,
                    name: "subsubnodeNameOne",
                    x: 8,
                    y: 8,
                    model: 1,
                    nodes: [],
                },
            ],
        }
    ],
    allComponents: [
        {name: "subnodeNameOne", id: null}, 
        {name: "newNode2", id: 35}, 
        {name: "subsubnodeNameOne", id: 4},
        {name: "subs222", id: 44}
    ],
})

export interface relationship {
    name: string,
    id: number,
    type: string,
    source: string,
    target: string
}

export interface position {
    x: number
    y: number 
}

export interface node {
    id: number | null,
    parent: number | null,
    name: string,
    x: number,
    y: number,
    model: number
    nodes: node[],
}

export interface modelType {
    id: number
    system: number
    type: string
    title: string
    raw_text: string
    time: string
}

export interface localModel {
    model: modelType,
    nodes: node[],
    allComponents: nameandid[],
}