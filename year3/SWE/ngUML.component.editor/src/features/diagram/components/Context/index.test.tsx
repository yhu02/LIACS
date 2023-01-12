import { fireEvent, getByTestId, render, screen, waitFor, } from 'tests/utils'
import { Context} from '.'

describe("Testing Context component", () => {

    beforeEach(() => {
        render(<Context />)
    });

    test("Should show Show context text", () => {
        expect(screen.getByText("Show context")).toBeDefined()
    })

    test("Should show message if button is clicked"), () => {
        //Isolate the button from the rendered component
        const button = screen.getByTestId("delete-subcomponent-button")

        //Before the button check if message is not displayed
        expect(screen.getByText("Are you sure you want to delete the subcomponent, this is an irreversible action.")).toBeUndefined()

        //Click the button
        fireEvent.click(button)
        
        //After the button check if message is displayed
        expect(screen.getByText("Are you sure you want to delete the subcomponent, this is an irreversible action.")).toBeDefined()

    }

    test("Should show Add Component text"), () => {
        expect(screen.getByText("Add Component")).toBeDefined()
    }
})


export default () => { };