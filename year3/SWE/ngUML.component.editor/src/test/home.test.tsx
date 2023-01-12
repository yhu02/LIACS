import { BrowserRouter } from 'react-router-dom'
import { render, screen } from 'tests/utils'
import { expect, it } from 'vitest'
import Index from 'pages/index'

it('Has the User portal banner', async () => {
    render(<BrowserRouter><Index /></BrowserRouter>)

    expect(screen.getByRole('banner', { name: 'ngUML User portal' })).toBeDefined()
})