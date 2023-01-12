import './_explainer.css'

import { Button, Column, Grid, Row, Tile } from 'carbon-components-react'

// @ts-ignore
import {ChevronRight} from '@carbon/icons-react'
import { Link } from "react-router-dom";
import React from 'react'

export const Explainer: React.FC = () => {
    return (
        <section>
            <Grid id="ExplainerContainer">
                <Column lg={12}>
                    <Row>
                        <h2>How it works</h2>
                    </Row>
                    <Row id="ExplainerText">
                        <p>Create UML diagrams from requirements texts using natural language processing techniques.</p>
                    </Row>
                    <Row>
                        <Link to="/requirements-preprocessing/project/select">
                            <Button renderIcon={ChevronRight} id="ExplainerButton">
                                Generate your models and prototypes
                            </Button>
                        </Link>
                    </Row>
                    <Row id="StepRow">
                        <Column lg={4} className="StepColumn">
                            <Tile className="Step">
                                <span className="StepCounter">
                                    1.
                                </span>
                                <h6 className="StepTitle">
                                    Gather your requirements
                                </h6>
                                <p>
                                    Write your requirements directly using our text editor, upload an audio file for automatic text to speech transliteration, or upload a plain text document to start with the process.
                                </p>
                            </Tile>
                        </Column>
                        <Column lg={4} className="StepColumn">
                            <Tile className="Step">
                                <span className="StepCounter">
                                    2.
                                </span>
                                <h6 className="StepTitle">
                                    Generate UML models
                                </h6>
                                <p>
                                    The requirements you provided will automatically be turned into UML class, activity and use case models. You keep in control of the modelling, we just make it easier to get a head start.
                                </p>
                            </Tile>
                        </Column>
                        <Column lg={4} className="StepColumn">
                            <Tile className="Step">
                                <span className="StepCounter">
                                    3.
                                </span>
                                <h6 className="StepTitle">
                                    Interact with your prototype
                                </h6>
                                <p>
                                    ngUML allows you to generate a running prototype of your class model, so that you can immediately start working with an application as if it was already built. 
                                </p>
                            </Tile>
                        </Column>
                    </Row>
                </Column>
            </Grid>
        </section>
    )
}

export default Explainer