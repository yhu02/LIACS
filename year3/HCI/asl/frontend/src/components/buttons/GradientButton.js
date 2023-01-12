import React from 'react';
import { Button } from "@mui/material";
import { useGradientBtnStyles } from '@mui-treasury/styles/button/gradient';

const GradientButton = () => {
  let chubbyStyles = useGradientBtnStyles({ chubby: false, background: 'linear-gradient(to right, #d4d4d4, #aaaaaa)' });
  return (
      <div>
        <Button classes={chubbyStyles}>Chubby</Button>
      </div>
  );
};


  export default GradientButton;
