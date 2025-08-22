#include "filters.h"

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
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = small_blur_filter.matrix;
        filter->bias = small_blur_filter.bias;
        filter->factor = small_blur_filter.factor;
        filter->size = SMALL_FILTER_SIZE;
        return filter;
    case MEDIUM:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = medium_blur_filter.matrix;
        filter->bias = medium_blur_filter.bias;
        filter->factor = medium_blur_filter.factor;
        filter->size = MEDIUM_FILTER_SIZE;
        return filter;
    case BIG:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = big_blur_filter.matrix;
        filter->bias = big_blur_filter.bias;
        filter->factor = big_blur_filter.factor;
        filter->size = BIG_FILTER_SIZE;
        return filter;
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
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = small_motion_blur_filter.matrix;
        filter->bias = small_motion_blur_filter.bias;
        filter->factor = small_motion_blur_filter.factor;
        filter->size = SMALL_FILTER_SIZE;
        return filter;
    case MEDIUM:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = medium_motion_blur_filter.matrix;
        filter->bias = medium_motion_blur_filter.bias;
        filter->factor = medium_motion_blur_filter.factor;
        filter->size = MEDIUM_FILTER_SIZE;
        return filter;
    case BIG:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = big_motion_blur_filter.matrix;
        filter->bias = big_motion_blur_filter.bias;
        filter->factor = big_motion_blur_filter.factor;
        filter->size = BIG_FILTER_SIZE;
        return filter;
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
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = small_find_edges_filter.matrix;
        filter->bias = small_find_edges_filter.bias;
        filter->factor = small_find_edges_filter.factor;
        filter->size = SMALL_FILTER_SIZE;
        return filter;
    case MEDIUM:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = medium_find_edges_filter.matrix;
        filter->bias = medium_find_edges_filter.bias;
        filter->factor = medium_find_edges_filter.factor;
        filter->size = MEDIUM_FILTER_SIZE;
        return filter;
    case BIG:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = big_find_edges_filter.matrix;
        filter->bias = big_find_edges_filter.bias;
        filter->factor = big_find_edges_filter.factor;
        filter->size = BIG_FILTER_SIZE;
        return filter;
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
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = small_emboss_filter.matrix;
        filter->bias = small_emboss_filter.bias;
        filter->factor = small_emboss_filter.factor;
        filter->size = SMALL_FILTER_SIZE;
        return filter;
    case MEDIUM:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = medium_emboss_filter.matrix;
        filter->bias = medium_emboss_filter.bias;
        filter->factor = medium_emboss_filter.factor;
        filter->size = MEDIUM_FILTER_SIZE;
        return filter;
    case BIG:
        convolution_filter *filter = malloc(sizeof(convolution_filter));
        if (!filter)
        {
            error("ERROR: malloc failed\n");
            return NULL;
        }
        filter->matrix = big_emboss_filter.matrix;
        filter->bias = big_emboss_filter.bias;
        filter->factor = big_emboss_filter.factor;
        filter->size = BIG_FILTER_SIZE;
        return filter;
    default:
        error("ERROR: invalid filter size");
        return NULL;
    }
}