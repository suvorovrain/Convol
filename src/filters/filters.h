#ifndef FILTERS_H
#define FILTERS_H

enum effect {
    BLUR = 1,
    MOTION_BLUR = 2,
    FIND_EDGES = 3,
    EMBOSS = 4,
};

enum strength
{
    SMALL = 1,
    MEDIUM = 2,
    BIG = 3,
};

#define SMALL_FILTER_SIZE 5
typedef struct {
    double matrix[SMALL_FILTER_SIZE][SMALL_FILTER_SIZE];
    double factor;
    double bias;
} small_convolution_filter;

extern small_convolution_filter small_blur_filter;

#define MEDIUM_FILTER_SIZE 7
typedef struct {
    double matrix[MEDIUM_FILTER_SIZE][MEDIUM_FILTER_SIZE];
    double factor;
    double bias;
} medium_convolution_filter;

extern medium_convolution_filter medium_blur_filter;

#define BIG_FILTER_SIZE 9
typedef struct {
    double matrix[BIG_FILTER_SIZE][BIG_FILTER_SIZE];
    double factor;
    double bias;
} big_convolution_filter;

extern big_convolution_filter big_blur_filter;

typedef struct {
    double **matrix;
    double factor;
    double bias;
    int size;
} convolution_filter;

#endif