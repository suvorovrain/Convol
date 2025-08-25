#include "../filters/filters.h"
#include "../utils/io.h"
#include "include/algo.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

image_data *parallel_pixel_convolution(image_data *image, convolution_filter *filter)
{
    image_data *new_image = create_canvas(image);
    if (!new_image)
    {
        free(new_image);
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
#pragma omp parallel for collapse(2)
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int fy = 0; fy < filter_size; fy++)
                for (int fx = 0; fx < filter_size; fx++)
                {
                    int ix = (x - filter_size / 2 + fx + width) % width;
                    int iy = (y - filter_size / 2 + fy + height) % height;
                    int idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            int idx = (y * width + x) * components;

            result_image[idx + 0] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + 1] = (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + 2] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    return new_image;
}

image_data *parallel_row_convolution(image_data *image, convolution_filter *filter)
{
    image_data *new_image = create_canvas(image);
    if (!new_image)
    {
        free(new_image);
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
#pragma omp parallel for
        for (int y = 0; y < height; y++)
        {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int fy = 0; fy < filter_size; fy++)
                for (int fx = 0; fx < filter_size; fx++)
                {
                    int ix = (x - filter_size / 2 + fx + width) % width;
                    int iy = (y - filter_size / 2 + fy + height) % height;
                    int idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            int idx = (y * width + x) * components;

            result_image[idx + 0] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + 1] = (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + 2] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    }
    return new_image;
}

image_data *parallel_column_convolution(image_data *image, convolution_filter *filter)
{
    image_data *new_image = create_canvas(image);
    if (!new_image)
    {
        free(new_image);
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
#pragma omp parallel for
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int fy = 0; fy < filter_size; fy++)
                for (int fx = 0; fx < filter_size; fx++)
                {
                    int ix = (x - filter_size / 2 + fx + width) % width;
                    int iy = (y - filter_size / 2 + fy + height) % height;
                    int idx = (iy * width + ix) * components;
                    red += image_src[idx + RED] * filter_mat[fy][fx];
                    green += image_src[idx + GREEN] * filter_mat[fy][fx];
                    blue += image_src[idx + BLUE] * filter_mat[fy][fx];
                }
            int idx = (y * width + x) * components;

            result_image[idx + 0] = (unsigned char)MIN(MAX((int)(factor * red + bias), 0), 255);
            result_image[idx + 1] = (unsigned char)MIN(MAX((int)(factor * green + bias), 0), 255);
            result_image[idx + 2] = (unsigned char)MIN(MAX((int)(factor * blue + bias), 0), 255);
        }
    return new_image;
}