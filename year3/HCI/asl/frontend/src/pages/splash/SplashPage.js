import { Helmet } from 'react-helmet-async';
import { Link } from 'react-router-dom';
// @mui
import { styled } from '@mui/material/styles';
import { Typography, Container, Box, Card } from '@mui/material';
import Logo from '../../components/logo';
import { Button2 } from '../../components/PageElements/UIElements';

// ----------------------------------------------------------------------

const StyledContent = styled('div')(({ theme }) => ({
  maxWidth: 480,
  margin: 'auto',
  minHeight: '100vh',
  display: 'flex',
  justifyContent: 'center',
  flexDirection: 'column',
  padding: theme.spacing(12, 0),
}));

// ----------------------------------------------------------------------

export default function SplashPage() {
  return (
    <>
      <Helmet>
        <title> SignUp! </title>
      </Helmet>

      <Container>
        <StyledContent sx={{ textAlign: 'center', alignItems: 'center' }}>
          <Logo />
          {/*<Link to="../../dashboard" style={{ textDecoration: 'none' }}>*/}
          <Link to="../intro" style={{ textDecoration: 'none' }}>
            {/*<Link to="../intro" style={{ textDecoration: 'none' }}>*/}

            <Button2 style={{
              marginTop: '100px',
              height: '200px', width: '400px',
              border: 'solid 7px #5f27cd',
              borderRadius: '20px',
              background: '#5f27cd00',
              marginTop: '50px'
            }}>
              <Typography variant="h1" sx={{ fontFamily: 'Righteous, cursive', color: '#5f27cd' }}>START</Typography>
            </Button2>
          </Link>
        </StyledContent>
      </Container>
    </>
  );
}
