import './../style.css';
import './_extractionreviewer.css';

import { Button, Column, ContentSwitcher, Grid, InlineNotification, Row, Switch, Tile } from "carbon-components-react";
import React, { useState } from "react";
// @ts-ignore
import { SkipBack, SkipForward } from '@carbon/icons-react'

import locale from './../../locales'
import { useNavigate } from 'react-router-dom'
import { usePreprocessingContext } from './../Shell'

export const ExtractionReviewer: React.FC = () => {
  let navigate = useNavigate();

  const preprocessingState = usePreprocessingContext()[0];

  let firstHighlightedType = 'class';
  //In the case of different highlighted types first.
  if (preprocessingState.extraction !== null) {
    firstHighlightedType = Object.keys(preprocessingState.extraction)[0]
  } 
  let [highlightedType, setHighlightedType] = useState(firstHighlightedType);

  return (
    <section>
      <Grid>
        <Row>
          <Column>
            <h1>{locale['preprocessing.extractionreviewer.title']}</h1>
          </Column>
        </Row>
        <Row>
          <Column lg={8} md={12}>
            <p>{locale['preprocessing.extractionreviewer.desc'].replace('%CURRENT_SYSTEM%', preprocessingState?.system?.name || 'System')}</p>
          </Column>
        </Row>
        <Row>
          <Column>
            <InlineNotification
              kind="info"
              subtitle={locale['preprocessing.extractionreviewer.info']}
              title=""
              hideCloseButton={true}
              id="EditNotification"
            />
          </Column>
        </Row>
        <Row>
          <Column>
            <Tile className="mt">
              <ContentSwitcher onChange={(content) => { setHighlightedType(String(content.name)) }}>
                {Object.keys(preprocessingState.extraction || {}).map((uml_type: string) => {
                  return (
                    <Switch name={uml_type} text={locale[`preprocessing.extractionreviewer.switcher.${uml_type}`]} />
                  )
                })}
              </ContentSwitcher>

              <Row id="MetadataField" className="mt">
                <Column lg={6}>
                  <label>
                    {locale['preprocessing.extractionreviewer.metadata.text.label']}
                  </label>
                  <p>
                    {preprocessingState && preprocessingState.extraction && preprocessingState.extraction[highlightedType]['text']}
                  </p>
                </Column>
                <Column lg={6}>
                  <label>
                    {locale['preprocessing.extractionreviewer.metadata.metadata.label']}
                  </label>
                  <p>
                    {preprocessingState && preprocessingState.extraction && preprocessingState?.extraction[highlightedType]['metadata']}
                  </p>
                </Column>
              </Row>
            </Tile>
          </Column>
        </Row>
        <Row>
          <Column>
            <div className="FlowNavigation">
              <Button renderIcon={SkipBack} kind="tertiary" onClick={() => navigate(-1)}>
                {locale['preprocessing.extractionreviewer.navigation.previous']}
              </Button>
              <Button renderIcon={SkipForward} onClick={() => navigate('/editor?from-preprocessing')}>
                {locale['preprocessing.extractionreviewer.navigation.next']}
              </Button>
            </div>
          </Column>
        </Row>
      </Grid>
    </section>
  );
};

export default ExtractionReviewer;
