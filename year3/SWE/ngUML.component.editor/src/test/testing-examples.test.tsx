import { fireEvent, getByTestId, render, screen } from 'tests/utils'
import {vi} from 'vitest'
import TestingExamples from 'pages/testing-examples'

//This is a testsuite that contains some example tests that can be used as templates
//for future tests in this project. 

describe("Dummy testsuite for some simple test examples for testing-examples.tsx ", () => {

    beforeEach(() => {
        //Run this code before every test in the suite
        //This renders the component and you can use screen as if you would be
        //looking at the component.
        //Note: you can use screen.debug() to see the HTML that is currently rendered
        //in the console.
        render(<TestingExamples />)
    });

    test("Should show text 'This text can be searched for' on the screen", () => {
        //You can use this functionality to
        expect(screen.getByText("This text can be searched for")).toBeDefined()
    })

    test("Text displayed should change on click of the button", () => {
        //Isolate the button from the rendered component,
        //this is very handy when you want to test single functionalities
        const button = screen.getByTestId("pressme-button")

        //Before the button check if before text is displayed
        expect(screen.getByText("Before pushing the button")).toBeDefined()

        //Click the button
        fireEvent.click(button)
        
        //After the button check if after text is displayed
        expect(screen.getByText("After pushing the button")).toBeDefined()
    })

    test("Error message should popup after clicking error button", () => {
        //Isolate the button from the rendered component,
        //this is very handy when you want to test single functionalities
        const button = screen.getByTestId("error-button")

        //Click the button
        fireEvent.click(button)

        //After the button check if error has appeared
        expect(screen.getByText("This is an error message!")).toBeDefined()
    })
})

export default () => { };