import PropTypes from 'prop-types';
// @mui
import { Box, Card, Link, Typography, Stack, CardContent, Container } from '@mui/material';
import { styled } from '@mui/material/styles';
// utils
import { fCurrency } from '../../../utils/formatNumber';
// components
import Label from '../../../components/label';
import { ColorPreview } from '../../../components/color-utils';
import { useTheme } from '@emotion/react';
import { Button } from '../../../components/PageElements/UIElements';

// ----------------------------------------------------------------------



export default function ShopProductCard({ topic, name, image, ...other }) {
  const theme = useTheme();
  return (
    <>

        <Button style={{ height: '100%', backgroundColor:'#bbaae5', border: 'solid 7px #5f27cd',
                                borderRadius: '20px',
                                }}>

          <Box
            component="img"
            src={'data:image/png;base64,' + image}
            sx={{
              height:'10vw',
              objectFit:'contain',
              maxHeight: '30vh',
              maxWidth: '30vh',
              margin: "auto auto",
              display: 'flex',
              justifyContent: 'center',
            }}
          // dangerouslySetInnerHTML={{ __html: image }}

          />


          <Box sx={{
            textAlign: 'center', // <-- the magic
            fontWeight: 'bold',
            fontSize: 30,
            color: '#0f0f0f',
            fontFamily: 'Righteous, cursive'
          }}>
            {name}
          </Box>
        </Button>


    </>
  );
}
