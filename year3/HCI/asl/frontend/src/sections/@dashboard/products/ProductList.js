import PropTypes from 'prop-types';
// @mui
import { Grid } from '@mui/material';
import ShopProductCard from './ProductCard';

// ----------------------------------------------------------------------


export default function ProductList({ products, data, toggle, ...other }) {
  let topics = []

  Object.keys(data).forEach(function (topic, index) {
    let files = []
    let item = {}
    item['topic'] = topic
    Object.keys(data[topic]).forEach(function (file, index2) {
      files.push(file)
    });
    item['files'] = files

    item['files'].sort()

    if (toggle == false) {
      item['files'].reverse()
    }
    topics.push(item)
  });

  return (
    <>
      <Grid container spacing={3} {...other} style={{ height: "100%" }}>

        {topics.map((topic) => (
          topic['files'].map((file) => (
            <Grid key={file} item xs={4}>
              <ShopProductCard name={file} data={data} image={data[topic['topic']][file]} />
            </Grid>
          ))
        ))}




      </Grid>
    </>
  );
}

/*
  {products.map((product) => (
          <Grid key={product.id} item xs={12} sm={6} md={3}>
            <ShopProductCard name={product.name} data={data} image={data[topic][name]} />
          </Grid>
        ))}
*/