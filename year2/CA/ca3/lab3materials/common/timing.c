/*
 * Skeleton code for use with Computer Architecture 2021 assignment 3,
 * LIACS, Leiden University.
 */

#include "timing.h"

#include <stdio.h>
#include <string.h>


/* Code taken from the GLIBC manual.
 *
 * Subtract the ‘struct timespec’ values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 */
int
timespec_subtract(struct timespec *result,
                  struct timespec *x,
                  struct timespec *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_nsec < y->tv_nsec) {
    int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
    y->tv_nsec -= 1000000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_nsec - y->tv_nsec > 1000000000) {
    int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
    y->tv_nsec += 1000000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_nsec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_nsec = x->tv_nsec - y->tv_nsec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

double
get_elapsed_ms(struct timespec *end_time,
               struct timespec *start_time)
{
  struct timespec elapsed_time;
  timespec_subtract(&elapsed_time, end_time, start_time);

  return (double)elapsed_time.tv_sec +
      (double)elapsed_time.tv_nsec / 1000000000.0;
}

void
print_elapsed_time(const char *description,
                   struct timespec *end_time,
                   struct timespec *start_time)
{
#ifdef ENABLE_TIMING
  double elapsed = get_elapsed_ms(end_time, start_time);
  fprintf(stderr, "%s: %f s\n", description, elapsed);
#endif /* ENABLE_TIMING */
}
