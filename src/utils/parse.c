
#include "../algo/include/algo.h"
#include "../filters/filters.h"
#include "parce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_MSG                                                                                  \
    "Usage: ./convol <src_image> <blur/motion_blur/find_edges/emboss> <1/2/3> <dest> "             \
    "<linear/parallel_pixel/parallel_row/parallel_column/split>\n"

#define error(...) (fprintf(stderr, __VA_ARGS__))

enum effect parse_effect(const char *str)
{
    if (strcmp(str, "blur") == 0)
        return BLUR;
    if (strcmp(str, "motion_blur") == 0)
        return MOTION_BLUR;
    if (strcmp(str, "find_edges") == 0)
        return FIND_EDGES;
    if (strcmp(str, "emboss") == 0)
        return EMBOSS;
    return -1;
}

enum algorithm_type parse_algorithm_type(const char *str)
{
    if (strcmp(str, "linear") == 0)
        return LINEAR;
    if (strcmp(str, "parallel_pixel") == 0)
        return PARALLEL_PIXEL;
    if (strcmp(str, "parallel_row") == 0)
        return PARALLEL_PIXEL;
    if (strcmp(str, "parallel_column") == 0)
        return PARALLEL_PIXEL;
    if (strcmp(str, "split") == 0)
        return SPLIT;
    return -1;
}

enum strength parse_effect_strength(const char *str)
{
    if (strcmp(str, "1") == 0)
        return SMALL;
    if (strcmp(str, "2") == 0)
        return MEDIUM;
    if (strcmp(str, "3") == 0)
        return BIG;
    return -1;
}

input_data *validate_input(int argc, char **argv)
{
    if (argc != 6)
    {
        error(USAGE_MSG);
        return NULL;
    };
    input_data *input = malloc(sizeof(*input));
    if (!input)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }
    int effect = parse_effect(argv[2]);
    if (effect == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    int strength = parse_effect_strength(argv[3]);
    if (strength == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    int algorithm = parse_algorithm_type(argv[5]);
    if (algorithm == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    input->src_image = argv[1];
    input->effect_type = effect;
    input->effect_strength = strength;
    input->folder_for_store = argv[4];
    input->algorithm = algorithm;
    return input;
}
