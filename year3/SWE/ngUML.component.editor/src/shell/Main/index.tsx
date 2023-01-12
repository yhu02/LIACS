import React from 'react'
import { EditorStateProvider } from '../../hooks/editorState'
import { EditorDataProvider } from '../../hooks/editorData'
import { TopMenu, NodeMenu, ContextMenu, Canvas, RuleMenu } from '..'
import './_main.css'
import ChatWindow from '../../chatbot/ChatWindow'
import { Button } from 'carbon-components-react'
import { MessageCircle } from 'react-feather'

export const Main: React.FC = () => {
    const [chat, setChat] = React.useState(false);

    return (
        <EditorStateProvider>
            <EditorDataProvider>
                <RuleMenu />
                <TopMenu />
                <NodeMenu />
                <ContextMenu/>
                <Canvas/>
                <ChatWindow open={chat} onClose={() => setChat(false)}/>
                <div style={{
                    position: 'absolute',
                    right: 32,
                    bottom: 32
                }}>
                    <Button
                        renderIcon={MessageCircle}
                        hasIconOnly
                        iconDescription="Chatbot"
                        onClick={() => setChat((chatState) => !chatState)}
                    />
                </div>
            </EditorDataProvider>
        </EditorStateProvider>
    )
}

export default Main