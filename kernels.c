#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"

/******************************************************
 * PINWHEEL KERNEL
 *
 * Your different versions of the pinwheel kernel go here
 ******************************************************/

/*
 * naive_pinwheel - The naive baseline version of pinwheel
 */
char naive_pinwheel_descr[] = "naive_pinwheel: baseline implementation";
void naive_pinwheel(pixel *src, pixel *dest)
{
  int i, j;

  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++)
    {
      /* Check whether we're in the diamond region */
      if ((fabs(i + 0.5 - src->dim / 2) + fabs(j + 0.5 - src->dim / 2)) < src->dim / 2)
      {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(src->dim - j - 1, i, src->dim);
        dest[d_idx].red = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      }
      else
      {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(i, j, src->dim);
        dest[d_idx] = src[s_idx];
      }
    }

}

/*
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
char pinwheel_descr[] = "pinwheel: Current working version";
void pinwheel(pixel *src, pixel *dest)
{
  int tile_size = 6;
  int image_dimension = src->dim;
  float half_dim = image_dimension / 2;
  float temp = 0.5 - half_dim;

  for (int ii = 0; ii < image_dimension; ii += tile_size) {
      for (int jj = 0; jj < image_dimension; jj += tile_size) {
          for (int i = ii; i < (ii + tile_size < image_dimension ? ii + tile_size : image_dimension); i++) {
              for (int j = jj; j < (jj + tile_size < image_dimension ? jj + tile_size : image_dimension); j++) {
                  /* Check whether we're in the diamond region */
                  if ((fabs(i + temp) + fabs(j + temp)) < half_dim)
                  {

                    /* In diamond region, so rotate and grayscale */
                    int s_idx = RIDX(i, j, image_dimension);

                    int d_idx = RIDX(image_dimension - j - 1, i, image_dimension);

                    int gray_value = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
                    dest[d_idx].red = gray_value;
                    dest[d_idx].green = gray_value;
                    dest[d_idx].blue = gray_value;

                  }
                  else
                  {
                    /* Not in diamond region, so keep the same */
                    int s_idx = RIDX(i, j, image_dimension);
                    int d_idx = RIDX(i, j, image_dimension);
                    dest[d_idx] = src[s_idx];

                  }
              }
          }
      }
  }
}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_pinwheel_functions()
{
  add_pinwheel_function(&pinwheel, pinwheel_descr);
  add_pinwheel_function(&naive_pinwheel, naive_pinwheel_descr);
}

/***************************************************************
 * GLOW KERNEL
 *
 * Starts with various typedefs and helper functions for the glow
 * function, and you may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct
{
  int red;
  int green;
  int blue;
} pixel_sum;

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
  sum->red = sum->green = sum->blue = 0;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_weighted_sum(pixel_sum *sum, pixel p, double weight)
{
  sum->red += (int)p.red * weight;
  sum->green += (int)p.green * weight;
  sum->blue += (int)p.blue * weight;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
  current_pixel->red = (unsigned short)sum.red;
  current_pixel->green = (unsigned short)sum.green;
  current_pixel->blue = (unsigned short)sum.blue;
}

/*
 * weighted_combo - Returns new pixel value at (i,j)
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  double weights[3][3] = {{0.16, 0.00, 0.16},
                          {0.00, 0.30, 0.00},
                          {0.16, 0.00, 0.16}};

  initialize_pixel_sum(&sum);
  for (ii = -1; ii < 2; ii++)
    for (jj = -1; jj < 2; jj++)
      if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim))
        accumulate_weighted_sum(&sum,
                                src[RIDX(i + ii, j + jj, dim)],
                                weights[ii + 1][jj + 1]);

  assign_sum_to_pixel(&current_pixel, sum);

  return current_pixel;
}

/******************************************************
 * Your different versions of the glow kernel go here
 ******************************************************/

/*
 * naive_glow - The naive baseline version of glow
 */
char naive_glow_descr[] = "naive_glow: baseline implementation";
void naive_glow(pixel *src, pixel *dst)
{
  int i, j;

  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++)
      dst[RIDX(i, j, src->dim)] = weighted_combo(src->dim, i, j, src);
}

/*
 * glow - Your current working version of glow.
 * IMPORTANT: This is the version you will be graded on
 */
char glow_descr[] = "glow: Current working version";

void glow(pixel *src, pixel *dst)
{
    int image_dimension = src->dim;
    int i, j;

    for (i = 0; i < image_dimension; i++) {
        for (j = 0; j < image_dimension; j++) {

            int s_idx = RIDX(i, j, image_dimension);
            int d_idx = RIDX(i, j, image_dimension);
            int bottom_right = RIDX(i + 1, j + 1, image_dimension);
            int bottom_left = RIDX(i + 1, j - 1, image_dimension);
            int top_left = RIDX(i - 1, j - 1, image_dimension);
            int top_right = RIDX(i - 1, j + 1, image_dimension);




            // Initialize the sum with the current pixel value

            int sum_red = (int)src[s_idx].red * 0.3;
            int sum_green = (int)src[s_idx].green * 0.3;
            int sum_blue = (int)src[s_idx].blue * 0.3;

            // Is it a corner pixel, an edge pixel or a regular pixel

            if ((i==0 && j==0) || (i == 0 && j == image_dimension - 1) || (i == image_dimension - 1 && j == 0) || (i == image_dimension - 1 && j == image_dimension - 1)){
              if (i == 0 && j == 0) {
                sum_red += (int)src[bottom_right].red * 0.16;
                sum_green += (int)src[bottom_right].green * 0.16;
                sum_blue += (int)src[bottom_right].blue * 0.16;
              }
              else if (i == 0 && j == image_dimension - 1){
                sum_red += (int)src[bottom_left].red * 0.16;
                sum_green += (int)src[bottom_left].green * 0.16;
                sum_blue += (int)src[bottom_left].blue * 0.16;
              }
              else if (i == image_dimension - 1 && j == 0){
                sum_red += (int)src[top_right].red * 0.16;
                sum_green += (int)src[top_right].green * 0.16;
                sum_blue += (int)src[top_right].blue * 0.16;
              }
              else {
                sum_red += (int)src[top_left].red * 0.16;
                sum_green += (int)src[top_left].green * 0.16;
                sum_blue += (int)src[top_left].blue * 0.16;

              }
            } else if (j == 0){
                sum_red += (int)src[top_right].red * 0.16;
                sum_green += (int)src[top_right].green * 0.16;
                sum_blue += (int)src[top_right].blue * 0.16;

                sum_red += (int)src[bottom_right].red * 0.16;
                sum_green += (int)src[bottom_right].green * 0.16;
                sum_blue += (int)src[bottom_right].blue * 0.16;
            } else if (j ==image_dimension-1){

                sum_red += (int)src[top_left].red * 0.16;
                sum_green += (int)src[top_left].green * 0.16;
                sum_blue += (int)src[top_left].blue * 0.16;

                sum_red += (int)src[bottom_left].red * 0.16;
                sum_green += (int)src[bottom_left].green * 0.16;
                sum_blue += (int)src[bottom_left].blue * 0.16;              
            } else if (i==0){

                sum_red += (int)src[bottom_left].red * 0.16;
                sum_green += (int)src[bottom_left].green * 0.16;
                sum_blue += (int)src[bottom_left].blue * 0.16;

                sum_red += (int)src[bottom_right].red * 0.16;
                sum_green += (int)src[bottom_right].green * 0.16;
                sum_blue += (int)src[bottom_right].blue * 0.16;

            }else if (i == image_dimension-1){

                sum_red += (int)src[top_left].red * 0.16;
                sum_green += (int)src[top_left].green * 0.16;
                sum_blue += (int)src[top_left].blue * 0.16;

                sum_red += (int)src[top_right].red * 0.16;
                sum_green += (int)src[top_right].green * 0.16;
                sum_blue += (int)src[top_right].blue * 0.16;

            } else {

                sum_red += (int)src[bottom_right].red * 0.16;
                sum_green += (int)src[bottom_right].green * 0.16;
                sum_blue += (int)src[bottom_right].blue * 0.16;

                sum_red += (int)src[bottom_left].red * 0.16;
                sum_green += (int)src[bottom_left].green * 0.16;
                sum_blue += (int)src[bottom_left].blue * 0.16;

                sum_red += (int)src[top_right].red * 0.16;
                sum_green += (int)src[top_right].green * 0.16;
                sum_blue += (int)src[top_right].blue * 0.16;

                sum_red += (int)src[top_left].red * 0.16;
                sum_green += (int)src[top_left].green * 0.16;
                sum_blue += (int)src[top_left].blue * 0.16;
                
            }
            // Assign the sum to destination pixel
            dst[d_idx].red = (unsigned short)sum_red;
            dst[d_idx].green = (unsigned short)sum_green;
            dst[d_idx].blue = (unsigned short)sum_blue;
          }
    }
}
/*********************************************************************
 * register_glow_functions - Register all of your different versions
 *     of the glow kernel with the driver by calling the
 *     add_glow_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_glow_functions()
{
  add_glow_function(&glow, glow_descr);
  add_glow_function(&naive_glow, naive_glow_descr);
}
