/*
 * Skeleton code for use with Computer Architecture 2021 assignment 3,
 * LIACS, Leiden University.
 */

#include "timing.h"

#include <immintrin.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <math.h>


#ifndef N_REPEAT
#  error "Need to define N_REPEAT in the Makefile"
#endif


/* Size of the matrix. We use N x N square matrices.
 *
 * We define a default here, if N was not set in the Makefile.
 */
#ifndef N
#  define N 512
#endif /* !defined N */

/* TODO: When you copy this file to implement loop blocking, you likely wan
 * to include a define for the block size as well.
 */


static void
matrix_clear(float matrix[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      matrix[i][j] = 0;
}

static void
matrix_init_identity(float matrix[N][N])
{
  matrix_clear(matrix);

  for (size_t i = 0; i < N; ++i)
    matrix[i][i] = 1.;
}

static void
matrix_init_random(float matrix[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      matrix[i][j] = rand() % 100;
}

void
matrix_dump(float matrix[N][N])
{
  for (size_t i = 0; i < N; ++i)
    {
      for (size_t j = 0; j < N; ++j)
        printf("%f ", matrix[i][j]);
      printf("\n");
    }
}

bool
matrix_equal(float A[N][N], float B[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      if (fabsf(A[i][j] - B[i][j]) > 0.000001)
        return false;

  return true;
}

static void
matrix_multiply(float C[N][N], const float A[N][N], const float B[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      {
        float row = 0.0;
        for (size_t k = 0; k < N; k++)
          row += A[i][k] * B[k][j];
        C[i][j] = row;
      }
}


int
main(int argc, char **argv)
{
  /* We use this "magic" to allocate real two-dimensional arrays, that
   * are aligned at 32-bytes. This is a requirement for AVX.
   */
  float (*A)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);
  float (*B)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);
  float (*C)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);

  struct timespec init_start_time, init_end_time;
  get_time(&init_start_time);

  matrix_init_identity(A);
  matrix_init_random(B);

  get_time(&init_end_time);

  /* Perform the matrix multiplication */
  struct timespec compute_start_time, compute_end_time;
  get_time(&compute_start_time);

  for (int Z = 0; Z < N_REPEAT; ++Z)
    matrix_multiply(C, A, B);

  get_time(&compute_end_time);

#ifdef ENABLE_TIMING
  /* Output result in CSV for easy processing */
  /* N,init time,n_repeat,runtime */
  printf("%d,%f,%d,%f\n",
         N,
         get_elapsed_ms(&init_end_time, &init_start_time),
         N_REPEAT,
         get_elapsed_ms(&compute_end_time, &compute_start_time));
#endif /* ENABLE_TIMING */

#if 0
  /* For debugging */
  if (!matrix_equal(C, B))
    fprintf(stderr, "Matrices are not equal!!\n");
#endif

  _mm_free(A);
  _mm_free(B);
  _mm_free(C);

  return 0;
}
