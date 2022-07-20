/*
 * Skeleton code for use with Computer Architecture 2021 assignment 3,
 * LIACS, Leiden University.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>
#include <stdbool.h>

/* Structure to hold image data and attributes. */
typedef struct
{
  int width;
  int height;
  int rowstride;

  uint32_t *data;
} image_t;


/* The functions are documented in image.c */

image_t *image_new              (const int      width,
                                 const int      height,
                                 const int      rowstride);
image_t *image_new_from_image   (const image_t *image);
image_t *image_new_from_image_T (const image_t *image);
void     image_free             (image_t       *image);

bool     image_equal_dimensions (const image_t *a,
                                 const image_t *b);

image_t *image_new_from_pngfile (const char    *filename);
bool     image_save_as_pngfile  (const image_t *image,
                                 const char    *filename);



/* By defining this as a macro, it can be used with both const and
 * non-const image pointers.
 */
#define image_get_pixel_data(data, rowstride, x, y) \
    &(data[(y) * ((rowstride)/sizeof(uint32_t)) + (x)])
#define image_get_pixel(image, x, y) \
    &((image)->data[(y) * ((image)->rowstride/sizeof(uint32_t)) + (x)])


/* Some convenient macros that operate on RGBA-vectors and arrays.
 * These can be used with both the rgba_t type in image.h, but also with the
 * float4 defined by CUDA
 *
 * Note: to ensure compatibility with CUDA's float4, we use
 * x,y,z,w instead of r,g,b,a (in that order).
 */

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

/* Simply a 4-vector of floats to hold one pixel.
 * Using CUDA, you could probably replace this with CUDA builti-in float4 type.
 * Note: to ensure compatibility with CUDA's float4, we use
 * x,y,z,w instead of r,g,b,a (in that order)
 */
typedef struct {
    float x,y,z,w; // r, g, b, a
} rgba_t;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Clamp `v` between `a` (lower bound) and `b` (upper bound), inclusive. */
#define CLAMP(v,a,b) \
    ((v)<(a) ? (a) : ((v)>(b) ? (b) : (v)))

/* Unpack an 32 bits integer to a 4-vector of floats */
#define RGBA_unpack(dst, src) \
(dst).x = (float)(((src) >> 24) & 0xff) / 255.f; \
(dst).y = (float)(((src) >> 16) & 0xff) / 255.f; \
(dst).z = (float)(((src) >> 8) & 0xff) / 255.f; \
(dst).w = (float)((src) & 0xff) / 255.f;

/* Pack a 4-vector of float into a single 32 bits integers */
#define RGBA_pack(dst, src) \
(dst) = \
    ((uint32_t)((src).x * 255) << 24) | \
    ((uint32_t)((src).y * 255) << 16) | \
    ((uint32_t)((src).z * 255) << 8) | \
    (uint32_t)((src).w * 255);


/* The functions below provide three simple vector operations (add, mult,
 * mults). Their current implementation is far from ideal. On the CPU we
 * would normally use SIMD instructions for these, while on the GPU you could
 * use the built-in vector types from CUDA.
 */

/* Construction: set vector `dst` to (r,g,b,a) */
#define RGBA(dst, r, g, b, a) \
    (dst).x =(r); (dst).y =(g); (dst).z =(b); (dst).w =(a);

/* Addition: add `a1` and `a2` component-wise and copy the result to `v`*/
#define RGBA_add(v, a1, a2) \
    (v).x = (a1).x + (a2).x; (v).y = (a1).y + (a2).y; (v).z = (a1).z + (a2).z; (v).w = (a1).w + (a2).w;

/* Multiplication: multiply `a1` and `a2` component-wise and copy the result to `v` */
#define RGBA_mult(v, a1, a2) \
    (v).x = (a1).x * (a2).x; (v).y = (a1).y * (a2).y; (v).z = (a1).z * (a2).z; (v).w = (a1).w * (a2).w; 

/* Multiplication by scalar: multiply `a1` by `s`, copy the result to `v` */
#define RGBA_mults(v, a1, s) \
    (v).x = (a1).x * (s); (v).y = (a1).y * (s); (v).z = (a1).z * (s); (v).w = (a1).w * (s);

/*! Clamp all components of `v' between `a' and `b' (inclusive) */
#define RGBA_clamp( v, a1, a, b ) \
    v.x = CLAMP( a1.x, a, b ); v.y = CLAMP( a1.y, a, b); v.z = CLAMP( a1.z, a, b ); v.w = CLAMP( a1.w, a, b );


#endif /* ! __IMAGE_H__ */
