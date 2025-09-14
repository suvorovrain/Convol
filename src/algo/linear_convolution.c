#include <stdio.h>
#include <stdlib.h>

#include "../filters/filters.h"
#include "../utils/include/io.h"
#include "include/algo.h"

image_data *linear_convolution(image_data *image, convolution_filter *filter)
{
    image_data *new_image = create_canvas(image);
    if (!new_image)
    {
        error("ERROR: new canvas creation failed\n");
        return NULL;
    };
    unsigned char *result_image = new_image->image;

    int width = image->width;
    int height = image->height;
    int components = image->components;
    unsigned char *image_src = image->image;

    double **filter_mat = filter->matrix;
    int filter_size = filter->size;
    double factor = filter->factor;
    double bias = filter->bias;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int fy = 0; fy < filter_size; fy++)
            {
                for (int fx = 0; fx < filter_size; fx++)
                {
                    int ix = x - filter_size / 2 + fx;
                    int iy = y - filter_size / 2 + fy;
                    if (ix < 0)
                    {
                        ix = 0;
                    };
                    if (iy < 0)
                    {
                        iy = 0;
                    };
                    if (ix >= width)
                    {
                        ix = width - 1;
                    };
                    if (iy >= height)
                    {
                        iy = height - 1;
                    };
                    int idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
                int idx = (y * width + x) * components;

                result_image[idx + RED] =
                    (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
                result_image[idx + GREEN] =
                    (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
                result_image[idx + BLUE] =
                    (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
            }
        }
    }
    return new_image;
}
