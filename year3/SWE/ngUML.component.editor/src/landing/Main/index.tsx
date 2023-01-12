import './_main.css'

import { Explainer, Footer, Hero, TopMenu } from '..'

import React from 'react'

export const Main: React.FC = () => {
    return (
        <>
            <TopMenu />
            <Hero />
            <Explainer />
            <Footer />
        </>
    )
}

export default Main
