#include "../../utils/io.h"
#include "../../filters/filters.h"

#ifndef PARALLEL_CONVOLUTION_H
#define PARALLEL_CONVOLUTION_H

image_data *prallel_pixel_convolution(image_data *image, convolution_filter *filter);
image_data *prallel_row_convolution(image_data *image, convolution_filter *filter);
image_data *prallel_column_convolution(image_data *image, convolution_filter *filter);

#endif // PARALLEL_CONVOLUTION_H 