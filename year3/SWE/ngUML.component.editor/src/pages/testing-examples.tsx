import React, { useState } from 'react'
import {
    Alert,
    Button,
} from '@blueprintjs/core'

type Props = {}

export const TestingExamples = (props: Props) => {
    //UseState makes sure that react is watching this variable for changes
    //It then updates the variable in the template
    const [text, setText] = useState("Before pushing the button")
    
    //Used for the error button to make sure it knows display the prompt
    const [delprompt, setDelprompt] = React.useState(false);

    //Function to change the state of the name variable
    //We call this function when pushing the PressMe! button
    const changeText = () => {
        setText("After pushing the button")
    }

    return (
        <div>
            {/* Headertext that is used in the first test in in "testing-examples.test.tsx" */}
            <h2>This text can be searched for</h2>
            <p> { text } </p>
            {/* //Button used for the second test in "testing-examples.test.tsx"
            //This button changes the text that is displayed from
            //"Before pushing the button" into "After pushing the button" */}
            <button 
                data-testid = "pressme-button"
                onClick={changeText}>Press Me!
            </button>

            {/* //Button used for the third test in "testing-examples.test.tsx"
            //This button is used to make an error alert appear. */}
            <Button
                data-testid = "error-button"
                intent='danger'
                icon='delete'
                onClick={() => setDelprompt(true)}
            />
            <Alert
                className='bp3-dark'
                intent='danger'
                confirmButtonText='OK'
                icon='delete'
                isOpen={delprompt}
                onClose={() => setDelprompt(false)}
            >
                This is an error message!
            </Alert>
        </div>
    )
}

export default TestingExamples