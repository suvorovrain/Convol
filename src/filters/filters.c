#include "filters.h"

const tiny_convolution_filter tiny_blur_filter = {
    .matrix = {
        {0.0, 1.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 1.0, 0.0},
    },
    .factor = 1.0 / 5.0,
    .bias = 0.0};

const small_convolution_filter small_blur_filter = {
    .matrix = {
        {0.0, 0.0, 1.0, 0.0, 0.0},
        {0.0, 1.0, 1.0, 1.0, 0.0},
        {1.0, 1.0, 1.0, 1.0, 1.0},
        {0.0, 1.0, 1.0, 1.0, 0.0},
        {0.0, 0.0, 1.0, 0.0, 0.0},
    },
    .factor = 1.0 / 5.0,
    .bias = 0.0};