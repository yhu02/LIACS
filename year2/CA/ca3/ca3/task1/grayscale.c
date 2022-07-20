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
 * Safe to use an in-place operation.
 */
void
op_grayscale(image_t *dst, const image_t *src)
{
  for (int x = 0; x < dst->width; x++)
    {
      for (int y = 0; y < dst->height; y++)
        {
          rgba_t color, gray;
          RGBA_unpack(color, *image_get_pixel(src, x, y));
          float intensity = compute_intensity(color);
          RGBA(gray, intensity, intensity, intensity, 1.f);
          RGBA_pack(*image_get_pixel(dst, x, y), gray);
        }
    }
}

void
op_grayscale2(image_t *dst, const image_t *src)
{
  for (int y = 0; y < dst->height; y++)
    {
    for (int x = 0; x < dst->width; x++)
        {
          rgba_t color, gray;
          RGBA_unpack(color, *image_get_pixel(src, x, y));
          float intensity = compute_intensity(color);
          RGBA(gray, intensity, intensity, intensity, 1.f);
          RGBA_pack(*image_get_pixel(dst, x, y), gray);
        }
    }
}


int
main(int argc, char **argv)
{
  if (argc < 3)
    {
      fprintf(stderr, "usage: %s <infile> <n_repeat> [outfile]\n", argv[0]);
      fprintf(stderr, "\n  where <infile> and [outfile] are PNG files.");
      fprintf(stderr, "\n  <n_repeat> is the number of times the computation should be repeated.");
      fprintf(stderr, "\n  [outfile] is an optional parameter.\n");
      return -1;
    }

  const char *infile = argv[1];
  long int n_repeat = 0;
  const char *outfile = argv[3];

  n_repeat = strtol(argv[2], NULL, 10);
  if (errno > 0 || n_repeat == 0)
    {
      fprintf(stderr, "Could not convert n_repeat argument to integer.\n");
      return EXIT_FAILURE;
    }

  /* Load PNG image */
  struct timespec load_start_time, load_end_time;
  get_time(&load_start_time);

  image_t *image = image_new_from_pngfile(infile);
  if (!image)
    return EXIT_FAILURE;

  get_time(&load_end_time);

#if ENABLE_TIMING
  //printf("filename,load time,grayscale time\n");
#endif /* ENABLE_TIMING */

  /* Execute */
  image_t *result = image_new_from_image(image);

  for (long int Z = 0; Z < n_repeat; Z++)
    {
      struct timespec start_time, end_time;
      struct timespec start_time2, end_time2;

      get_time(&start_time);
      //op_grayscale2(result, image); compare with this one
      op_grayscale(result, image);
      get_time(&end_time);

#if ENABLE_TIMING
      /* Output result in CSV for easy processing */
      /* filename,load time,compute time */
      printf("%s,%f,%f\n",
             get_basename(infile),
             get_elapsed_ms(&load_end_time, &load_start_time),
             get_elapsed_ms(&end_time, &start_time));
#endif /* ENABLE_TIMING */
    }

  /* Save result if desired and if applicable */
  int retval = EXIT_SUCCESS;
  if (outfile && result && !image_save_as_pngfile(result, outfile))
    retval = EXIT_FAILURE;

  image_free(image);
  image_free(result);

  return retval;
}
