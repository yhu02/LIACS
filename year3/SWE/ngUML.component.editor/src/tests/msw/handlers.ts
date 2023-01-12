import { rest } from 'msw'

export const projects = [
]

export const handlers = [
    rest.get('http://localhost:8000/api/model/project', (req, res, ctx) => {
        return res(ctx.status(200), ctx.json(projects))
    }),

    rest.get('http://localhost:8000/api/model/project/1/system/1/1/', (req, res, ctx) => {
        return res(ctx.status(200), ctx.json({
        }))
    }),

    //Mocked API calls for src//features/diagram/components/NodeSelector/index.test.tsx
    rest.get('http://localhost:8000/api/model/project/1/system/1/class/', (req, res, ctx) => {
        return res(ctx.status(404), ctx.json({}))
    }),

    // rest.get('http://localhost:8000/api/model/project/1/system/1/activity/', (req, res, ctx) => {
    //     return res(ctx.status(200), ctx.json({
    //         "nodes": [],
    //         "connections": []}))
    // }),
]