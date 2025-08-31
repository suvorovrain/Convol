#include "../../utils/include/io.h"
#include "../../filters/filters.h"

#ifndef PARALLEL_CONVOLUTION_H
#define PARALLEL_CONVOLUTION_H

image_data *parallel_pixel_convolution(image_data *image, convolution_filter *filter);
image_data *parallel_row_convolution(image_data *image, convolution_filter *filter);
image_data *parallel_column_convolution(image_data *image, convolution_filter *filter);

#endif // PARALLEL_CONVOLUTION_H 