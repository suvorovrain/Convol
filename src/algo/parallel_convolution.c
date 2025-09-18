#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "../filters/filters.h"
#include "../utils/include/io.h"
#include "include/algo.h"

image_data *parallel_pixel_convolution(image_data *image, convolution_filter *filter)
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
    double red = 0.0, green = 0.0, blue = 0.0;
    int x, y, fx, fy, ix, iy, idx;
#pragma omp parallel for collapse(2)                                                               \
    schedule(static) private(x, y, fx, fy, ix, iy, idx, red, green, blue)
    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            red = 0.0, green = 0.0, blue = 0.0;
            for (fy = 0; fy < filter_size; fy++)
            {
                for (fx = 0; fx < filter_size; fx++)
                {
                    ix = x - filter_size / 2 + fx;
                    iy = y - filter_size / 2 + fy;

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

                    idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            }
            idx = (y * width + x) * components;

            result_image[idx + RED] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + GREEN] =
                (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + BLUE] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    }
    return new_image;
}

image_data *parallel_row_convolution(image_data *image, convolution_filter *filter)
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
    double red = 0.0, green = 0.0, blue = 0.0;
    int x, y, fx, fy, ix, iy, idx;
    for (x = 0; x < width; x++)
    {
#pragma omp parallel for schedule(static) private(y, fx, fy, ix, iy, idx, red, green, blue)
        for (y = 0; y < height; y++)
        {
            red = 0.0, green = 0.0, blue = 0.0;
            for (fy = 0; fy < filter_size; fy++)
            {
                for (fx = 0; fx < filter_size; fx++)
                {
                    ix = x - filter_size / 2 + fx;
                    iy = y - filter_size / 2 + fy;
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

                    idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            }
            idx = (y * width + x) * components;

            result_image[idx + RED] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + GREEN] =
                (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + BLUE] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    }
    return new_image;
}

image_data *parallel_column_convolution(image_data *image, convolution_filter *filter)
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
    double red = 0.0, green = 0.0, blue = 0.0;
    int x, y, fx, fy, ix, iy, idx;
#pragma omp parallel for schedule(static) private(x, y, fx, fy, ix, iy, idx, red, green, blue)
    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            red = 0.0, green = 0.0, blue = 0.0;
            for (fy = 0; fy < filter_size; fy++)
            {
                for (fx = 0; fx < filter_size; fx++)
                {
                    ix = x - filter_size / 2 + fx;
                    iy = y - filter_size / 2 + fy;
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

                    idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            }
            idx = (y * width + x) * components;

            result_image[idx + RED] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + GREEN] =
                (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + BLUE] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    }
    return new_image;
}
