import { Helmet } from 'react-helmet-async';
import { Link } from 'react-router-dom';
// @mui
import { styled } from '@mui/material/styles';
import { useState, useEffect } from 'react'
import { Typography, Container, Box, Card, Pagination, Grid, Button } from '@mui/material';
import Welcome from '../../components/welcome';

import { Button2} from '../../components/PageElements/UIElements';
import $ from 'jquery'
import Iconify from '../../components/iconify';

// ----------------------------------------------------------------------

const StyledContent = styled('div')(({ theme }) => ({
  maxWidth: '100%',
  margin: 'auto',
  minHeight: '100vh',
  display: 'flex',
  justifyContent: 'center',
  flexDirection: 'column',
  padding: theme.spacing(12, 0),
}));


// ----------------------------------------------------------------------

export default function SplashPage() {
  const [page, setPage] = useState('1')
  let size = 22;
  let margin_ = 20;
  let chevronColor = '#969696'
  useEffect(() => {
    $('#3').css("background-color", '#5f27cd')
  }, [])

  let toggle = true

  return (
    <>
      <Helmet>
        <title> SignUp! </title>
      </Helmet>
      <Grid container>
        <Grid item xs={3}>
          <Box sx={{ display: 'inline-block' }} style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '110vh'}}>
          <Link to="../intro3" style={{ textDecoration: 'none' }}>
            <Button
              color="inherit"
              disableRipple
              endIcon={<Iconify icon='eva:chevron-left-fill' sx={{ width: '100%', height: '200px', color: chevronColor }} />}
            >
            </Button>
            </Link>
          </Box>
        </Grid>
        <Grid item xs={6}>
          <StyledContent sx={{ textAlign: 'center', alignItems: 'center' }}>
            <Welcome></Welcome>

            <div style={{ marginBottom: '100px' }}>
              <Typography variant="h1" fontFamily='righteous' style={{ margin: '50px' }}> Do a Scroll</Typography>
              <Typography variant="subtitle1" fontSize='26px'> Extend only your index and middle finger </Typography>
              <Typography variant="subtitle1" fontSize='26px'> Do a circle motion anti-clockwise with your index finger, </Typography>
              <Typography variant="subtitle1" fontSize='26px'> without moving the rest of your hand </Typography>


            </div>


            <div class="pagination">
              {[...Array(4)].map((x, i) =>
                <Box id={i}
                  style={{
                    width: size,
                    height: size,
                    borderRadius: size / 2, backgroundColor: '#d8d8d8',
                    display: 'inline-block', margin: margin_
                  }}></Box>
              )}

            </div>         </StyledContent>
        </Grid>
        <Grid item xs={3}>
          <Grid container>
            <Grid item xs={10}>
              <Link to="../../dashboard" style={{ textDecoration: 'none' }}>
                  <Button2 style={{
                    height: '100%',
                    width: '100%',
                    border: 'solid 7px #5f27cd',
                    borderRadius: '20px',
                    background: '#5f27cd00',
                    marginTop:'50px'
                  }}>
                    <Typography variant="h1" sx={{ fontFamily: 'Righteous, cursive', color: '#5f27cd' }}>SKIP</Typography>
                  </Button2>
              </Link>
            </Grid>
          </Grid>
          <Box sx={{ display: 'inline-block' }} style={{ display: 'none', justifyContent: 'center', alignItems: 'center', height: '80vh', marginLeft:'200px' }}>
          <Link to="../intro4" style={{ textDecoration: 'none' }}>
            <Button
              color="inherit"
              disableRipple
              endIcon={<Iconify icon='eva:chevron-right-fill' sx={{ width: '100%', height: '200px', color: chevronColor }} />}
            >
            </Button>
            </Link>
          </Box>
        </Grid >
      </Grid >
    </>
  );
}


