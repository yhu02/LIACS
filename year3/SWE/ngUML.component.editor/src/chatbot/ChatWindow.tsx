import React from 'react'
import { createPortal }from 'react-dom'
import { ComposedModal, Modal, ModalBody, ModalHeader } from 'carbon-components-react'
import ChatDialogue from './ChatDialogue';

interface IChatWindow {
    title?: string;
    open: boolean;
    onClose?(): boolean | void;
}

const ChatWindow: React.FC<IChatWindow> = ({
    title,
    open,
    onClose
}) => {
    return <>
        {
            createPortal(
                <ComposedModal
                    open={open}
                    onClose={onClose}
                >
                    <ModalHeader>{ title ?? 'ngUML Chatbot' }</ModalHeader>
                    <div>
                        <ChatDialogue/>
                    </div>
                </ComposedModal>,
                document.body
            )
        }
    </>
}

export default ChatWindow