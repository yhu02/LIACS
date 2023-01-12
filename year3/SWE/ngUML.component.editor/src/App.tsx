import React from 'react'
import 'carbon-components/css/carbon-components.min.css'

import { QueryClient, QueryClientProvider } from '@tanstack/react-query'
import { ReactQueryDevtools } from '@tanstack/react-query-devtools'
import { BrowserRouter as Router, useRoutes } from 'react-router-dom'
import routes from '~react-pages'
import { queryClient } from './api'
import './App.scss'
import { Loading } from 'carbon-components-react'

const RouterApp: React.FC = () => {
    return <React.Suspense fallback={<Loading />}>
        {useRoutes(routes)}
    </React.Suspense>
}

const App: React.FC = () => {
    return (
        <React.StrictMode>
            <QueryClientProvider client={queryClient}>
                <Router>
                    <>
                        <RouterApp />
                    </>
                </Router>
                <ReactQueryDevtools initialIsOpen={false} />
            </QueryClientProvider>
        </React.StrictMode>
    )
}

export default App