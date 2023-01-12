import { BrowserRouter } from 'react-router-dom'
import { render } from 'tests/utils'
import { it } from 'vitest'
import Editor from 'pages/editor'

it('Renders', async () => {
    render(<BrowserRouter><Editor /></BrowserRouter>)
})

export default () => { };