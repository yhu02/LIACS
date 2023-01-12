import { useState } from 'react';
// @mui
import { Menu, Button, MenuItem, Typography } from '@mui/material';
// component
import Iconify from '../../../components/iconify';

// ----------------------------------------------------------------------

const SORT_BY_OPTIONS = [
  { value: 'asc', label: 'A-Z' },
  { value: 'desc', label: 'Z-A' },
];

export default function ShopProductSort({ toggle }) {

  return (
    <>

      <Button
        color="inherit"
        disableRipple
        endIcon={<Iconify icon={toggle ? 'eva:chevron-up-fill' : 'eva:chevron-down-fill'} sx={{ width: '100%', height: '200px',color: '#0db2ad' }} />}
      >
        <Typography component="span" variant="h1" sx={{ color: '#0db2ad', fontFamily: 'Righteous, cursive' }}>
          {toggle ? 'A-Z' : 'Z-A'}
        </Typography>

      </Button>

    </>
  );
}
