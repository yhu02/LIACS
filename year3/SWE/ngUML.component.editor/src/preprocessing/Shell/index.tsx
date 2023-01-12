import './_shell.css'

import {
    Header,
    HeaderMenuItem,
    HeaderName,
    HeaderNavigation,
    ProgressIndicator,
    ProgressStep
} from 'carbon-components-react'
import { Outlet, useLocation, useOutletContext } from 'react-router-dom'
import React, { useState } from 'react'

import { PreprocessingState } from './../../hooks/preprocessingState'
import locale from './../../locales'

export default function Shell() {
    const location = useLocation();

    const [preprocessingState, setPreprocessingState] = useState<PreprocessingState | null>({
        project: null,
        system: null,
        extraction: null,
    });

    // Dynamically set step counter based on route
    let currentStep: number;
    const splittedRoute: string[] = location.pathname.split('/');
    switch(splittedRoute[splittedRoute.length - 1]) {
        case 'system':
            currentStep = 1;
            break;
        case 'review':
            currentStep = 2;
            break;
        default:
            currentStep = 0;
    }

    return (
        <main>
            <Header aria-label={`${locale['preprocessing.shell.header.prefix']} ${locale['preprocessing.shell.header.title']}`}>
                <HeaderName prefix={locale['preprocessing.shell.header.prefix']}>
                    {locale['preprocessing.shell.header.title']}
                </HeaderName>
                <HeaderNavigation aria-label={`${locale['preprocessing.shell.header.prefix']} [${locale['preprocessing.shell.header.title']}]`}>
                    <HeaderMenuItem href="/">
                        {locale['preprocessing.shell.header.menu.home']}
                    </HeaderMenuItem>
                    <HeaderMenuItem href="http://localhost:8000/requirement/">
                        {locale['preprocessing.shell.header.menu.managerequirements']}
                    </HeaderMenuItem>
                    <HeaderMenuItem href="http://localhost:8000/model/">
                        {locale['preprocessing.shell.header.menu.runnableprototype']}
                    </HeaderMenuItem>
                </HeaderNavigation>
            </Header>

            <div id="ContentContainer">
                <aside id="ProgressSidebar">
                    <h6>
                        {locale['preprocessing.shell.sidebar.title']}
                    </h6>
                    
                    <ProgressIndicator currentIndex={currentStep} vertical>
                        {Array.from({length: 3}, (v, i) => i + 1).map((stepNo: number) => {
                            return (
                                <ProgressStep className="ProgressStep"
                                    label={locale[`preprocessing.shell.sidebar.steps.step${stepNo}.title`]}
                                    description={`${locale[`preprocessing.shell.sidebar.steps.step${stepNo}.desc`]}: ${locale[`preprocessing.shell.sidebar.steps.step${stepNo}.title`]}`}
                                    secondaryLabel={locale[`preprocessing.shell.sidebar.steps.step${stepNo}.desc`]}
                                />
                            )
                        })}
                    </ProgressIndicator>
                </aside>

                <Outlet context={[preprocessingState, setPreprocessingState]} />
            </div>
        </main>
    )
}

export function usePreprocessingContext() {
    return useOutletContext<[PreprocessingState, any]>();
}