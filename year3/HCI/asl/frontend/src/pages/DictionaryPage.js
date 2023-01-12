import { Helmet } from 'react-helmet-async';
import { useEffect, useState } from 'react';
// @mui
import { Container, Stack, Typography, CardMedia, Grid, Card } from '@mui/material';
// components
import { ProductSort, ProductList, ProductCartWidget, ProductFilterSidebar } from '../sections/@dashboard/products';
// mock
import $ from 'jquery';

import { Button } from '../components/PageElements/UIElements';

// ----------------------------------------------------------------------
let dictionary_data;
const dictionary_data_call = () => {

  $.ajax({
    type: 'GET',
    url: 'http://localhost:8000/dictionary/',
    data: {},
    success: function (data) {
      dictionary_data = data;
    },
    dataType: '',
    async: false
  });

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
var scrollableElement = document.body; //document.getElementById('scrollableElement');

scrollableElement.addEventListener('wheel', checkScrollDirection);

var block = false;

function checkScrollDirection(event) {
  if (checkScrollDirectionIsUp(event)) {
    return ('up')
  } else {
    return ('down')
  }
}

function checkScrollDirectionIsUp(event) {
  if (event.wheelDelta) {
    return event.wheelDelta > 0;
  }
  return event.deltaY < 0;
}

var keys = { 37: 1, 38: 1, 39: 1, 40: 1 };

function preventDefault(e) {
  e.preventDefault();
  if (block == false) {
    var temp = checkScrollDirection(e);
    if (temp == 'up') {
      document.documentElement.scrollTop -= 300
    } else if (temp == 'down') {
      document.documentElement.scrollTop += 300
    }
    block = true;
    setTimeout(() => {block = false}, 1000);
  }
}

function preventDefaultForScrollKeys(e) {
  if (keys[e.keyCode]) {
    preventDefault(e);
    return false;
  }
}

// modern Chrome requires { passive: false } when adding event
var supportsPassive = false;
try {
  window.addEventListener("test", null, Object.defineProperty({}, 'passive', {
    get: function () { supportsPassive = true; }
  }));
} catch (e) { }

var wheelOpt = supportsPassive ? { passive: false } : false;
var wheelEvent = 'onwheel' in document.createElement('div') ? 'wheel' : 'mousewheel';

// call this to Disable
function disableScroll() {
  window.addEventListener(wheelEvent, preventDefault, wheelOpt); // modern desktop
}

// call this to Enable
function enableScroll() {
  window.removeEventListener(wheelEvent, preventDefault, wheelOpt);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

export default function DictionaryPage() {
  const [openFilter, setOpenFilter] = useState(false);

  const [toggle, setToggle] = useState(false);
  const [toggle2, setToggle2] = useState(false);
  disableScroll()
  //enableScroll()
  useEffect(() => {
  }, [toggle2]);

  useEffect(() => {
  }, [toggle]);
  const handleOpenFilter = () => {
    setOpenFilter(true);
  };

  const handleCloseFilter = () => {
    setOpenFilter(false);
  };

  dictionary_data_call()


  return (
    <>
      <Helmet>
        <title> SignUp! </title>
      </Helmet>
      <Container>
        <Typography variant="h4" sx={{ mb: 5 }}>
          Dictionary
        </Typography>
        <Grid container sx={{ paddingBottom: '50px' }}>
          <Grid item xs={5} onClick={(e) => { setToggle(!toggle) }}>
            <Card>
              <Button style={{ backgroundColor: '#5f27cd' }}>
                <ProductSort toggle={toggle} />
              </Button>
            </Card>
          </Grid>
          <Grid item xs={2}>
          </Grid>
          <Grid item xs={5}>
            <Card>
              <Button style={{ backgroundColor: '#5f27cd' }}>
                <ProductFilterSidebar
                  openFilter={openFilter}
                  onOpenFilter={handleOpenFilter}
                  onCloseFilter={handleCloseFilter}
                  data={dictionary_data}
                  toggle={setToggle2}
                />
              </Button>
            </Card>
          </Grid>

        </Grid>

        <ProductList data={dictionary_data} toggle={toggle} />
      </Container>
    </>
  );
}
