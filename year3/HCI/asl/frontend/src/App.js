// routes
import Router from './routes';
// theme
import ThemeProvider from './theme';
// components
import ScrollToTop from './components/scroll-to-top';
import { StyledChart } from './components/chart';
import CssBaseline from '@mui/material/CssBaseline';
import React from 'react'
import axios from 'axios'


const theme = {
  primary: "#6849ff",
  secondary: "#36eabb",
  success: "#35e9b3",
  error: "#e93574",
  gradient: ["#7f00ff", "#e100ff"],
  textDark1: "#141414",
  text3: "#cccccc",
  text2: "#ebebeb",
  text1: "#fcfcfc",
  warning: "#fbc72c",
  surface1: "#1f1047",
  surface2: '#2a165b',
  surface3: '#361f71',
  borderRadius: '14px',
  transition: 'all ease 0.25s',
  boxShadow: '0 8px 30px rgb(0 0 0 / 15%);',
  padding: '8pt calc(16pt + 0.25rem)'
}

export default function App() {

  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <ScrollToTop />
      <StyledChart />
      <Router />
    </ThemeProvider>
  );
}


