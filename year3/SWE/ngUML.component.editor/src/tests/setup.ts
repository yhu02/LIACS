import { server } from 'tests/msw/server'
import { fetch } from 'cross-fetch';

global.fetch = fetch

beforeAll(() => server.listen({ onUnhandledRequest: 'error' }))
afterAll(() => server.close())
afterEach(() => server.resetHandlers())