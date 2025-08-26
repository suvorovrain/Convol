#include "include/algo.h"
#include "../filters/filters.h"
#include "../utils/io.h"
#include "include/linear_convolution.h"
#include "include/parallel_convolution.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define error(...) (fprintf(stderr, __VA_ARGS__))

image_data *convolution(image_data *image, convolution_filter *filter, int type)
{
    switch (type)
    {
    case LINEAR:
        return linear_convolution(image, filter);
    case PARALLEL_PIXEL:
        return parallel_pixel_convolution(image, filter);
    case PARALLEL_ROW:
        return parallel_row_convolution(image, filter);
    case PARALLEL_COLUMN:
        return parallel_column_convolution(image, filter);
    case SPLIT:
        error("TODO: implement split algorithm\n");
        return NULL;
    default:
        error("ERROR: unsupported algorithm type\n");
        return NULL;
    }
}

image_data *create_canvas(image_data *image)
{
    image_data *out = malloc(sizeof(*out));
    if (!out)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }

    out->width = image->width;
    out->height = image->height;
    out->components = image->components;

    size_t size = (size_t)out->width * out->height * out->components;
    out->image = malloc(size);
    if (!out->image)
    {
        error("ERROR: malloc failed\n");
        free(out);
        return NULL;
    };
    return out;
}