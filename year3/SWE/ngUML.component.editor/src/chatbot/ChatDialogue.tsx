import { Button, TextInput, Loading } from "carbon-components-react";
import React from "react";
import { Send } from "react-feather";
import styled from "styled-components";

const OuterTile = styled.div`
  margin: 1em 1em 0 1em;
  padding: 1em;
  height: 384px;
  overflow-y: auto;
  display: flex;
  flex-direction: column-reverse;
  width: auto;
  background-color: #fff;
`;

const SentMessage = styled.div`
  padding: 1em;
  margin: 0.5em;
  background-color: lightgray;
  align-self: flex-end;
  position: relative;

  &:after {
    content: "";
    width: 0;
    height: 0;
    border-style: solid;
    border-width: 0 6px 6px 0;
    border-color: transparent lightgray transparent transparent;
    right: 0;
    bottom: -6px;
    position: absolute;
  }
`;

const ReceivedMessage = styled.div`
  padding: 1em;
  margin: 0.5em;
  background-color: blue;
  color: white;
  align-self: flex-start;
  position: relative;

  &:after {
    content: "";
    width: 0;
    height: 0;
    border-style: solid;
    border-width: 0 0 6px 6px;
    border-color: transparent transparent transparent blue;
    left: 0;
    bottom: -6px;
    position: absolute;
  }
`;

const InputField = styled.form`
  margin: 0 1em 1em 1em;
  display: flex;
`;

const ChatDialogue = () => {
  const [message, setMessage] = React.useState("");
  const [dialogue, setDialogue] = React.useState<any[]>([]);
  const [loading, setLoading] = React.useState(false);

  const sendMessage = (e: any) => {
    e.preventDefault();
    setLoading(true);

    let formData = new FormData(); //formdata object
    formData.append("text_input", message);
    formData.append("form_type", "formTwo");
    fetch("http://localhost:8000/", {
      method: "POST",
      body: formData,
    })
      .then((response) => response.json())
      .then((data) => {
        setDialogue([
          {
            message: data["antwoord"],
            type: "received",
          },
          {
            message: message,
            type: "sent",
          },
          ...dialogue,
        ]);
      });

    setLoading(false);
    setMessage("");
  };

  return (
    <>
      <OuterTile>
          {loading ? <Loading/> : <></>}
        {dialogue.map(({ type, message }) => {
          return type == "sent" ? (
            <SentMessage>{message}</SentMessage>
          ) : (
            <ReceivedMessage>{message}</ReceivedMessage>
          );
        })}
      </OuterTile>
      <InputField onSubmit={sendMessage}>
        <TextInput
          id="one"
          labelText="Chat Message..."
          hideLabel
          placeholder="Send a chat message..."
          size="xl"
          value={message}
          onChange={(e) => {
            setMessage(e.target.value);
          }}
          required
        />
        <Button
          type="submit"
          renderIcon={Send}
          hasIconOnly
          iconDescription="Send Message"
        />
      </InputField>
    </>
  );
};

export default ChatDialogue;
