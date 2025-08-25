
#include "../algo/include/algo.h"
#include "../filters/filters.h"
#include "parce.h"
#include "stdlib.h"

input_data *validate_input(int argc, char **argv)
{
    if (argc != 6)
    {
        error("Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen/emboss> "
              "<1/2/3> <dest> <linear/parallel_pixel/parallel_row/parallel_column/split>\n");
        return NULL;
    };
    input_data *input = malloc(sizeof(*input));
    if (!input)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }
    int effect = parse_effect(argv[2]);
    if (effect != 0)
    {
        error("Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen/emboss> "
              "<1/2/3> <dest> <linear/parallel_pixel/parallel_row/parallel_column/split>\n");
        return NULL;
    };
    int strength = parse_effect_strength(argv[3]);
    if (strength != 0)
    {
        error("Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen/emboss> "
              "<1/2/3> <dest> <linear/parallel_pixel/parallel_row/parallel_column/split>\n");
        return NULL;
    };
    int algorithm = parse_algorithm_type(argv[5]);
    if (algorithm != 0)
    {
        error("Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen/emboss> "
              "<1/2/3> <dest> <linear/parallel_pixel/parallel_row/parallel_column/split>\n");
        return NULL;
    };
    input->src_image = argv[1];
    input->effect_type = effect;
    input->effect_strength = strength;
    input->folder_for_store = argv[4];
    input->algorithm = algorithm;
    return input;
}

enum effect parse_effect(const char *str)
{
    printf("parse_effect\n");
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
    printf("parse_algo\n");
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
    printf("parse_strength\n");
    if (strcmp(str, "1") == 0)
        return SMALL;
    if (strcmp(str, "2") == 0)
        return MEDIUM;
    if (strcmp(str, "3") == 0)
        return BIG;
    return -1;
}