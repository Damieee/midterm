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
