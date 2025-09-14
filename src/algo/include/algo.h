#include "../../utils/include/io.h"
#include "../../filters/filters.h"
#include "../../utils/include/parse.h"
#include <stdio.h>

#ifndef ALGO_H
#define ALGO_H

#define READERS_NUMBER 5
#define WRITERS_NUMBER 5
#define WORKERS_NUMBER 10
#define IN_QUEUE_SIZE 4 * WORKERS_NUMBER
#define OUT_QUEUE_SIZE 2 * WORKERS_NUMBER

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define RED 0
#define GREEN 1
#define BLUE 2

image_data *create_canvas(image_data *image);

image_data *convolution(image_data *image, convolution_filter *filter, int type);
int single_convolution(input_data *input, convolution_filter *filter);
int stream_convolution(input_data *input, convolution_filter *filter);



#endif // ALGO_H
