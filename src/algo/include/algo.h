#include "../../utils/io.h"
#include "../../filters/filters.h"

#ifndef ALGO_H
#define ALGO_H

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define RED 0
#define GREEN 1
#define BLUE 2
enum algorithm_type
{
    LINEAR = 1,
    PARALLEL_PIXEL = 2,
    PARALLEL_ROW = 3,
    PARALLEL_COLUMN = 4,
    SPLIT = 5,
};

image_data *convolution(image_data *image, convolution_filter *filter, int type);

#endif // ALGO_H