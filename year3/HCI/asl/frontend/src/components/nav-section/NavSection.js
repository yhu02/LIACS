import PropTypes from 'prop-types';
import { NavLink as RouterLink } from 'react-router-dom';
import { Box, List, ListItemButton, ListItemText, Button, Typography } from '@mui/material';
import { StyledNavItem, StyledNavItemIcon } from './styles';
import { Link, Routes, Route, useNavigate } from 'react-router-dom';

// ----------------------------------------------------------------------

NavSection.propTypes = {
  data: PropTypes.array,
};

export default function NavSection({ data = [], ...other }) {
  return (
    <Box {...other} display="flex"
    justifyContent="center"
    alignItems="center" >

      <List disablePadding sx={{ p: 1 }}>
        {data.map((item) => (
          <NavItem key={item.title} item={item} />
        ))}
      </List>
    </Box>
  );
}

// ----------------------------------------------------------------------

NavItem.propTypes = {
  item: PropTypes.object,
};

function NavItem({ item }) {
  const { title, path, icon, info } = item;

  return (
    <>
      <ListItemButton href={item.path} divider={true} sx={{ paddingTop: 5, paddingBottom: 5 }}>
        <StyledNavItemIcon>{icon && icon}</StyledNavItemIcon>

        <Typography variant="h3">{item.title}</Typography>
      </ListItemButton>

    </>
  );
}
