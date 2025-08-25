#include "utils/io.h"
#include "filters/filters.h"
#include "include/algo.h"
#include "stdlib.h"
image_data *convolution(image_data *image, convolution_filter *filter, int type){
    switch (type)
    {
    case LINEAR:
        
        break;
    case PARALLEL_PIXEL:
        error("TODO: implement parallel_pixel algorithm\n");
        return NULL;
    case PARALLEL_ROW:
        error("TODO: implement parallel_row algorithm\n");
        return NULL;
    case PARALLEL_COLUMN:
        error("TODO: implement parallel_column algorithm\n");
        return NULL;
    case SPLIT:
        error("TODO: implement parallel_split algorithm\n");
        return NULL;
    default:
        error("ERROR: unsupported algorithm type\n");
        return NULL;
    }
}