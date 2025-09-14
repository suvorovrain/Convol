#include "../utils/include/parse.h"
#include <stdio.h>

#ifndef FILTERS_H
#define FILTERS_H

#define error(...) (fprintf(stderr, __VA_ARGS__))

#define SMALL_FILTER_SIZE 5
typedef struct
{
    double matrix[SMALL_FILTER_SIZE][SMALL_FILTER_SIZE];
    double factor;
    double bias;
} small_convolution_filter;

#define MEDIUM_FILTER_SIZE 7
typedef struct
{
    double matrix[MEDIUM_FILTER_SIZE][MEDIUM_FILTER_SIZE];
    double factor;
    double bias;
} medium_convolution_filter;

#define BIG_FILTER_SIZE 9
typedef struct
{
    double matrix[BIG_FILTER_SIZE][BIG_FILTER_SIZE];
    double factor;
    double bias;
} big_convolution_filter;

typedef struct
{
    double **matrix;
    double factor;
    double bias;
    int size;
} convolution_filter;

void free_convolution_filter(convolution_filter *f);
convolution_filter *set_filter(enum effect eff, enum strength strength);

convolution_filter *create_blur_convolution_filter(enum strength str);
convolution_filter *create_motion_blur_convolution_filter(enum strength str);
convolution_filter *create_find_edges_convolution_filter(enum strength str);
convolution_filter *create_emboss_convolution_filter(enum strength str);

#endif
