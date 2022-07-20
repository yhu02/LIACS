/*
 * Skeleton code for use with Computer Architecture 2021 assignment 3,
 * LIACS, Leiden University.
 */

#include "timing.h"
#include "image.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

static inline const char *
get_basename(const char *str)
{
  const char *end = strrchr(str, '/');
  if (!end)
    return str;
  return end + 1;
}


/* Computes the intensity of @color. To do so, we use CIE 1931 weights
 * multiplied by alpha: Y = A( 0.2126R + 0.7152G + 0.0722B ).
 */
static float
compute_intensity(rgba_t color)
{
  return color.w * (0.2126f * color.x + 0.7152f * color.y + 0.0722f * color.z);
}

/* Computes the grayscale value for each pixel in @src and stores this in @dst.
 * @dst is expected to have been created already with the correct dimensions.
 * Safe to use a in-place operation.
 */
void
op_grayscale(image_t *dst, const image_t *src, int x, int y)
{

          rgba_t color, gray;
          RGBA_unpack(color, *image_get_pixel(src, x, y));
          float intensity = compute_intensity(color);
          RGBA(gray, intensity, intensity, intensity, 1.f);
          RGBA_pack(*image_get_pixel(dst, x, y), gray);
}


/* Tiles the @tile image on @background using alpha blending. For the tile
 * an alpha value of @tile_alpha is used.
 */
void
op_tile_composite(image_t *background,
                  const image_t *tile, const float tile_alpha, int x, int y)
{

          int tx = x % tile->width;
          int ty = y % tile->height;

          rgba_t dst, src;
          RGBA_unpack(dst, *image_get_pixel(background, x, y));
          RGBA_unpack(src, *image_get_pixel(tile, tx, ty));
          RGBA_mults(src, src, tile_alpha);
          RGBA_mults(dst, dst, 1.f - tile_alpha);
          RGBA_add(dst, dst, src);
          RGBA_pack(*image_get_pixel(background, x, y), dst);

}



int
main(int argc, char **argv)
{
  if (argc < 4)
    {
      fprintf(stderr, "usage: %s <infile> <tilefile> <n_repeat> [outfile]\n", argv[0]);
      fprintf(stderr, "\n  where <infile>, <tilefile> and [outfile] are PNG files.");
      fprintf(stderr, "\n  <n_repeat> is the number of times the computation should be repeated.");
      fprintf(stderr, "\n  [outfile] is an optional parameter.\n");
      return -1;
    }

  const char *infile = argv[1];
  const char *tilefile = argv[2];
  long int n_repeat = 0;
  const char *outfile = argv[4];

  n_repeat = strtol(argv[3], NULL, 10);
  if (errno > 0 || n_repeat == 0)
    {
      fprintf(stderr, "Could not convert n_repeat argument to integer.\n");
      return EXIT_FAILURE;
    }

  /* Load PNG images */
  struct timespec load_start_time, load_end_time;
  get_time(&load_start_time);

  image_t *background = image_new_from_pngfile(infile);
  if (!background)
    return EXIT_FAILURE;

  image_t *tile = image_new_from_pngfile(tilefile);
  if (!tile)
    return EXIT_FAILURE;

  get_time(&load_end_time);

#if ENABLE_TIMING
  //printf("filename,tile filename,load time,pipeline1 time\n");
#endif /* ENABLE_TIMING */
  if(tile->width > background->width) {
    exit(0);
  }
  int block = tile->width;
  int blockWidth = background->width / block;

  /* Execute */
  for (long int z = 0; z < n_repeat; z++)
    {
      
      struct timespec start_time, end_time;
      get_time(&start_time);
      for (int y = 0; y < background->height; y += block)
        {
          
        for (int x = 0; x < background->width; x += block * blockWidth)
          {
            for(int blockRow = y; blockRow < y + block; blockRow++){
              for(int blockCol = x; blockCol < x + block * blockWidth; blockCol++){
          
                op_tile_composite(background, tile, 0.2f,  blockCol, blockRow);
                
                op_grayscale(background, background,  blockCol, blockRow); /* in-place */
                
              }
            }
          }
        }
      get_time(&end_time);

#if ENABLE_TIMING
      /* Output result in CSV for easy processing */
      /* filename,tile filename,load time,compute time */
      printf("%s,%s,%f,%f\n",
             get_basename(infile), get_basename(tilefile),
             get_elapsed_ms(&load_end_time, &load_start_time),
             get_elapsed_ms(&end_time, &start_time));
#endif /* ENABLE_TIMING */
    }


  /* Save result if desired and if applicable */
  int retval = EXIT_SUCCESS;
  if (outfile && background && !image_save_as_pngfile(background, outfile))
    retval = EXIT_FAILURE;

  image_free(background);
  image_free(tile);

  return retval;
}
