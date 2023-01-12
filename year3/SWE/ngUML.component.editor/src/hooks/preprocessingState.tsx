export type ProjectData = {
    id: number | null;
    name: string | null;
    desc: string | null;
    requirements: string | null;
    topics: string[] | null;
}

export type SystemData = {
    id: number | null;
    name: string | null;
    entities: string[];
    types: string[];
}

export type ExtractionData = {
    [key: string]: {
        text: string;
        metadata: string;
        model_id: number;
    }
}

export type PreprocessingState = {
    project: null | ProjectData;
    system: null | SystemData;
    extraction: null | ExtractionData;
}