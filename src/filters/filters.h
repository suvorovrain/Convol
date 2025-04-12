#ifndef FILTERS_H
#define FILTERS_H

#define FLYWEIGHT_FILTER_WIDTH 3
#define FLYWEIGHT_FILTER_HEIGHT 3

typedef struct
{
    double matrix[FLYWEIGHT_FILTER_HEIGHT][FLYWEIGHT_FILTER_WIDTH];
    double factor;
    double bias;
} tiny_convolution_filter;

extern tiny_convolution_filter tiny_blur_filter;

#define SMALL_FILTER_WIDTH 5
#define SMALL_FILTER_HEIGHT 5
    typedef struct
{
    double matrix[FLYWEIGHT_FILTER_HEIGHT][FLYWEIGHT_FILTER_WIDTH];
    double factor;
    double bias;
} small_convolution_filter;

extern small_convolution_filter small_blur_filter;

#define MEDIUM_FILTER_WIDTH 7
#define MEDIUM_FILTER_HEIGHT 7
    typedef struct
{
    double matrix[FLYWEIGHT_FILTER_HEIGHT][FLYWEIGHT_FILTER_WIDTH];
    double factor;
    double bias;
} medium_convolution_filter;

extern medium_convolution_filter medium_blur_filter;

#define BIG_FILTER_WIDTH 9
#define BIG_FILTER_HEIGHT 9
    typedef struct
{
    double matrix[FLYWEIGHT_FILTER_HEIGHT][FLYWEIGHT_FILTER_WIDTH];
    double factor;
    double bias;
} big_convolution_filter;

extern big_convolution_filter big_blur_filter;

#define HUGE_FILTER_WIDTH 15
#define HUGE_FILTER_HEIGHT 15
    typedef struct
{
    double matrix[FLYWEIGHT_FILTER_HEIGHT][FLYWEIGHT_FILTER_WIDTH];
    double factor;
    double bias;
} huge_convolution_filter;

extern huge_convolution_filter huge_blur_filter;

#endif