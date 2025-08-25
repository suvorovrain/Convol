#include "utils/io.h"
enum algorithm_type
{
    LINEAR = 1,
    PARALLEL_PIXEL = 2,
    PARALLEL_ROW = 3,
    PARALLEL_COLUMN = 4,
    SPLIT = 5,
};

image_data *convolution(image_data *image, convolution_filter *filter, int type);