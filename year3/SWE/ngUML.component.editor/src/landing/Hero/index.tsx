import './_hero.css'

import { Button, Column, Grid, Row } from 'carbon-components-react'

import { ReactComponent as BackgroundIllustration } from './BackgroundIllustration.svg';
// @ts-ignore
import {ChevronRight} from '@carbon/icons-react'
import { Link } from "react-router-dom";
import React from 'react'

export const Hero: React.FC = () => {
    return (
        <section id="Hero">
            <Grid id='HeroContainer'>
                <Column lg={4} sm={16}>
                    <Row>
                        <h1>Generate UML diagrams, the easy way.</h1>
                    </Row>
                    <Row>
                        <h4>Create UML diagrams from requirements texts using natural language processing techniques.</h4>
                    </Row>
                    <Row>
                        <Link to="/requirements-preprocessing/project/select">
                            <Button renderIcon={ChevronRight} id="HeroButton">
                                Get started
                            </Button>
                        </Link>
                    </Row>
                </Column>
                
                <BackgroundIllustration id="BackgroundIllustration" />
            </Grid>
        </section>
    )
}

export default Hero