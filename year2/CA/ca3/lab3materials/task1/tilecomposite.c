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

/* Tiles the @tile image on @background using alpha blending. For the tile
 * an alpha value of @tile_alpha is used.
 */
void
op_tile_composite(image_t *background,
                  const image_t *tile, const float tile_alpha)
{
  for (int y = 0; y < background->height; y++)
    {
      for (int x = 0; x < background->width; x++)
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
    }
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
      return EXIT_FAILURE;
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
  printf("filename,tile filename,load time,tilecomposite time\n");
#endif /* ENABLE_TIMING */

  /* Execute */
  for (long int Z = 0; Z < n_repeat; Z++)
    {
      struct timespec start_time, end_time;

      get_time(&start_time);

      op_tile_composite(background, tile, 0.2f);

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
