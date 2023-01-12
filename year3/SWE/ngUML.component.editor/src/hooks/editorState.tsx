import React from 'react'

type IEditorState = {
    scale: number,
    setScale: (value: number) => void;
    panning: boolean,
    setPanning: (value: boolean) => void;
    focus: string | undefined;
    setFocus: (value: string | undefined) => void;
    redraw: number;
    setRedraw: (value: number) => void;
}

const EditorState = React.createContext<
    IEditorState | undefined
>(undefined)

export const EditorStateProvider: React.FC = ({children}: React.PropsWithChildren) => {
    // State-hook that is toggles if a click should pan the
    // surface or if we should ignore a click for panning.
    const [panning, setPanning] = React.useState<boolean>(true)
    const [scale, setScale] = React.useState<number>(1)
    const [focus, setFocus] = React.useState<string>()
    const [redraw, setRedraw] = React.useState<number>(new Date().getTime())

    return (
        <EditorState.Provider
            value={{
                scale,
                setScale,
                panning,
                setPanning,
                focus,
                setFocus,
                redraw,
                setRedraw
            }}
        >
            {children}
        </EditorState.Provider>
    )
}

export default EditorState