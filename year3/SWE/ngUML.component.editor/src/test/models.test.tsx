import { BrowserRouter } from 'react-router-dom'
import { render } from 'tests/utils'
import { it } from 'vitest'
import Models from 'pages/models'
import { queryClient } from 'api'
import { QueryClientProvider } from '@tanstack/react-query'

it('Renders', async () => {
    render(<QueryClientProvider client={queryClient}><BrowserRouter><Models /></BrowserRouter></QueryClientProvider>)
})