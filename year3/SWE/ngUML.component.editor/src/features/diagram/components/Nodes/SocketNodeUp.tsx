// Description: Socket node up

// Import the necessary libraries
import { memo } from 'react';
import { Handle } from 'reactflow';
import styled from 'styled-components';

// Define the socket node
const Node = styled.div`
  background: ${(props) => props.theme.nodeBg};
  color: ${(props) => props.theme.nodeColor};
  text-align: center;
  width: 25px;
  height: 25px;
  border: 1px solid black;
  border-radius: 50%;
  border-bottom-color: transparent;
  border-left-color: transparent;
  transform: rotate(135deg);

  .react-flow__handle {
    background: rgba(240,240,240,0);
    width: 0px;
    height: 0px;
    border-radius: 100px;
    border-color: rgba(240,240,240,0);
  }
`;

export default memo(({ data, selected }: any) => {
    return (
        <Node selected={selected}>
            <Handle type="target" position="top"  style={{ top: "100%" }} />
            <Handle type="source" position="bottom" style={{ top: "20%" }} />
        </Node>
    );
});