#include "../../algo/include/algo.h"
#include <stdio.h>

#ifndef PARCE_H
#define PARCE_H

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef struct
{
    char *src_image;
    enum effect effect_type;
    enum strength effect_strength;
    char *folder_for_store;
    enum algorithm_type algorithm;
} input_data;

input_data *validate_input(int argc, char **argv);
#endif // PARCE_H
