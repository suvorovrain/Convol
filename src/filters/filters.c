#include "filters.h"

#include <stdio.h>
#include <stdlib.h>

static convolution_filter *alloc_and_copy_2d(int size, double factor, double bias,
                                             const double src[][size])
{
    convolution_filter *f = malloc(sizeof(*f));
    if (!f)
    {
        fprintf(stderr, "ERROR: malloc failed\n");
        return NULL;
    }

    f->matrix = malloc((size_t)size * sizeof(double *));
    if (!f->matrix)
    {
        free(f);
        fprintf(stderr, "ERROR: malloc failed\n");
        return NULL;
    }

    for (int i = 0; i < size; ++i)
    {
        f->matrix[i] = malloc((size_t)size * sizeof(double));
        if (!f->matrix[i])
        {
            for (int k = 0; k < i; ++k)
            {
                free(f->matrix[k]);
            }
            free(f->matrix);
            free(f);
            fprintf(stderr, "ERROR: malloc failed\n");
            return NULL;
        }
        for (int j = 0; j < size; ++j)
        {
            f->matrix[i][j] = src[i][j];
        }
    }
    f->size = size;
    f->factor = factor;
    f->bias = bias;
    return f;
}

void free_convolution_filter(convolution_filter *filter)
{
    if (!filter)
    {
        return;
    }

    for (int i = 0; i < filter->size; ++i)
    {
        free(filter->matrix[i]);
    }

    free(filter->matrix);
    free(filter);
}

convolution_filter *set_filter(enum effect eff, enum strength strength)
{
    switch (eff)
    {
    case BLUR:
        return create_blur_convolution_filter(strength);
    case MOTION_BLUR:
        return create_motion_blur_convolution_filter(strength);
    case FIND_EDGES:
        return create_find_edges_convolution_filter(strength);
    case EMBOSS:
        return create_emboss_convolution_filter(strength);
    default:
        error("ERROR: invalid filter name");
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------
// Blur
//----------------------------------------------------------------------------------------------

const small_convolution_filter small_blur_filter = {.matrix =
                                                        {
                                                            {0.0, 0.0, 1.0, 0.0, 0.0},
                                                            {0.0, 1.0, 1.0, 1.0, 0.0},
                                                            {1.0, 1.0, 1.0, 1.0, 1.0},
                                                            {0.0, 1.0, 1.0, 1.0, 0.0},
                                                            {0.0, 0.0, 1.0, 0.0, 0.0},
                                                        },
                                                    .factor = 1.0 / 13.0,
                                                    .bias = 0.0};
const medium_convolution_filter medium_blur_filter = {.matrix =
                                                          {
                                                              {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                                                              {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0},
                                                              {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0},
                                                              {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                                                              {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0},
                                                              {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0},
                                                              {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},

                                                          },
                                                      .factor = 1.0 / 25.0,
                                                      .bias = 0.0};
const big_convolution_filter big_blur_filter = {
    .matrix =
        {
            {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0},
            {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
            {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0},
            {0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
        },
    .factor = 1.0 / 41.0,
    .bias = 0.0};

convolution_filter *create_blur_convolution_filter(enum strength str)
{
    switch (str)
    {
    case SMALL:
        return alloc_and_copy_2d(SMALL_FILTER_SIZE, small_blur_filter.factor,
                                 small_blur_filter.bias, small_blur_filter.matrix);
    case MEDIUM:
        return alloc_and_copy_2d(MEDIUM_FILTER_SIZE, medium_blur_filter.factor,
                                 medium_blur_filter.bias, medium_blur_filter.matrix);
    case BIG:
        return alloc_and_copy_2d(BIG_FILTER_SIZE, big_blur_filter.factor, big_blur_filter.bias,
                                 big_blur_filter.matrix);
    default:
        error("ERROR: invalid filter size");
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------
// Motion blur
//----------------------------------------------------------------------------------------------

const small_convolution_filter small_motion_blur_filter = {.matrix =
                                                               {
                                                                   {1.0, 0.0, 0.0, 0.0, 0.0},
                                                                   {0.0, 1.0, 0.0, 0.0, 0.0},
                                                                   {0.0, 0.0, 1.0, 0.0, 0.0},
                                                                   {0.0, 0.0, 0.0, 1.0, 0.0},
                                                                   {0.0, 0.0, 0.0, 0.0, 1.0},
                                                               },
                                                           .factor = 1.0 / 5.0,
                                                           .bias = 0.0};
const medium_convolution_filter medium_motion_blur_filter = {
    .matrix =
        {
            {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},

        },
    .factor = 1.0 / 7.0,
    .bias = 0.0};
const big_convolution_filter big_motion_blur_filter = {
    .matrix =
        {
            {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
        },
    .factor = 1.0 / 9.0,
    .bias = 0.0};

convolution_filter *create_motion_blur_convolution_filter(enum strength str)
{
    switch (str)
    {
    case SMALL:
        return alloc_and_copy_2d(SMALL_FILTER_SIZE, small_motion_blur_filter.factor,
                                 small_motion_blur_filter.bias, small_motion_blur_filter.matrix);
    case MEDIUM:
        return alloc_and_copy_2d(MEDIUM_FILTER_SIZE, medium_motion_blur_filter.factor,
                                 medium_motion_blur_filter.bias, medium_motion_blur_filter.matrix);
    case BIG:
        return alloc_and_copy_2d(BIG_FILTER_SIZE, big_motion_blur_filter.factor,
                                 big_motion_blur_filter.bias, big_motion_blur_filter.matrix);
    default:
        error("ERROR: invalid filter size");
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------
// Find edges
//----------------------------------------------------------------------------------------------

const small_convolution_filter small_find_edges_filter = {.matrix =
                                                              {
                                                                  {-1.0, -1.0, -1.0, -1.0, -1.0},
                                                                  {-1.0, -1.0, -1.0, -1.0, -1.0},
                                                                  {-1.0, -1.0, 24.0, -1.0, -1.0},
                                                                  {-1.0, -1.0, -1.0, -1.0, -1.0},
                                                                  {-1.0, -1.0, -1.0, -1.0, -1.0},
                                                              },
                                                          .factor = 1.0,
                                                          .bias = 0.0};
const medium_convolution_filter medium_find_edges_filter = {
    .matrix =
        {
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, 48.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},

        },
    .factor = 1.0,
    .bias = 0.0};
const big_convolution_filter big_find_edges_filter = {
    .matrix =
        {
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, 80.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0},
        },
    .factor = 1.0,
    .bias = 0.0};

convolution_filter *create_find_edges_convolution_filter(enum strength str)
{
    switch (str)
    {
    case SMALL:
        return alloc_and_copy_2d(SMALL_FILTER_SIZE, small_find_edges_filter.factor,
                                 small_find_edges_filter.bias, small_find_edges_filter.matrix);
    case MEDIUM:
        return alloc_and_copy_2d(MEDIUM_FILTER_SIZE, medium_find_edges_filter.factor,
                                 medium_find_edges_filter.bias, medium_find_edges_filter.matrix);
    case BIG:
        return alloc_and_copy_2d(BIG_FILTER_SIZE, big_find_edges_filter.factor,
                                 big_find_edges_filter.bias, big_find_edges_filter.matrix);
    default:
        error("ERROR: invalid filter size");
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------
// Emboss
//----------------------------------------------------------------------------------------------

const small_convolution_filter small_emboss_filter = {.matrix =
                                                          {
                                                              {-1.0, -1.0, -1.0, -1.0, 0.0},
                                                              {-1.0, -1.0, -1.0, 0.0, 1.0},
                                                              {-1.0, -1.0, 0.0, 1.0, 1.0},
                                                              {-1.0, 0.0, 1.0, 1.0, 1.0},
                                                              {0.0, 1.0, 1.0, 1.0, 1.0},
                                                          },
                                                      .factor = 1.0,
                                                      .bias = 128.0};
const medium_convolution_filter medium_emboss_filter = {
    .matrix =
        {
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 1.0},
            {-1.0, -1.0, -1.0, -1.0, 0.0, 1.0, 1.0},
            {-1.0, -1.0, -1.0, 0.0, 1.0, 1.0, 1.0},
            {-1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0},
            {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0},
            {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},

        },
    .factor = 1.0,
    .bias = 128.0};
const big_convolution_filter big_emboss_filter = {
    .matrix =
        {
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 1.0, 1.0},
            {-1.0, -1.0, -1.0, -1.0, -1.0, 0.0, 1.0, 1.0, 1.0},
            {-1.0, -1.0, -1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0},
            {-1.0, -1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0},
            {-1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
            {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
            {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
        },
    .factor = 1.0,
    .bias = 128.0};

convolution_filter *create_emboss_convolution_filter(enum strength str)
{
    switch (str)
    {
    case SMALL:
        return alloc_and_copy_2d(SMALL_FILTER_SIZE, small_emboss_filter.factor,
                                 small_emboss_filter.bias, small_emboss_filter.matrix);
    case MEDIUM:
        return alloc_and_copy_2d(MEDIUM_FILTER_SIZE, medium_emboss_filter.factor,
                                 medium_emboss_filter.bias, medium_emboss_filter.matrix);
    case BIG:
        return alloc_and_copy_2d(BIG_FILTER_SIZE, big_emboss_filter.factor, big_emboss_filter.bias,
                                 big_emboss_filter.matrix);
    default:
        error("ERROR: invalid filter size");
        return NULL;
    }
}
