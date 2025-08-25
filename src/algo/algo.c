#include "../utils/io.h"
#include "../filters/filters.h"
#include "include/algo.h"
#include "include/linear_convolution.h"
#include "include/parallel_convolution.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define error(...)(fprintf(stderr, __VA_ARGS__))

image_data *convolution(image_data *image, convolution_filter *filter, int type){
    switch (type)
    {
    case LINEAR:
        return linear_convolution(image,filter);
    case PARALLEL_PIXEL:
        return parallel_pixel_convolution(image,filter);
    case PARALLEL_ROW:
        return parallel_row_convolution(image,filter);
    case PARALLEL_COLUMN:
        return parallel_column_convolution(image,filter);
    case SPLIT:
        error("TODO: implement split algorithm\n");
        return NULL;
    default:
        error("ERROR: unsupported algorithm type\n");
        return NULL;
    }
}
