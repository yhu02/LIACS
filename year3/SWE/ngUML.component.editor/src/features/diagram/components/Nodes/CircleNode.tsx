// Description: Lollipop node

// Import the necessary libraries
import { memo } from 'react';
import { Handle } from 'reactflow';
import styled from 'styled-components';

// Define the lollipop node
const Node = styled.div`
  padding: 10px 10px;
  border-radius: 100px;
  background: ${(props) => props.theme.nodeBg};
  color: ${(props) => props.theme.nodeColor};
  width: 10px;
  height: 10px;
  text-align: center;
  border: 1px solid ${(props) => (props.selected ? props.theme.primary : props.theme.nodeBorder)};
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
            <Handle type="source" position="left" />
            <Handle type="target" position="right" />
        </Node>
    );
});