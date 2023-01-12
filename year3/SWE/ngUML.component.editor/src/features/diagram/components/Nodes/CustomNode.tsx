// Description: Custom node -> component

// Import the necessary libraries
import { memo } from 'react';
import { Handle } from 'reactflow';
import styled from 'styled-components';

// Define the component node
const Node = styled.div`
  padding: 10px 20px;
  background: rgba(0,0,0,.16);
  color: black;
  text-align: center;

  .react-flow__handle {
    background: rgba(240,240,240,1);
    margin: -1px;
    width: 10px;
    height: 25px;
    border-radius: 1px;
    border-color: black;
  }
`;

export default memo(({ data, selected } : any) => {
  return (
    <Node selected={selected}>
      <Handle type="source" position="left" />
      <div>
        <p>{data.label}</p>
      </div>
      <Handle type="target" position="right" />
    </Node>
  );
});
