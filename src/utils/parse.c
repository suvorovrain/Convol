
#include "include/parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../algo/include/algo.h"
#include "../filters/filters.h"

#define USAGE_MSG                                                                                  \
    "Usage: ./convol <src_image> <blur/motion_blur/find_edges/emboss> <1/2/3> <dest> "             \
    "<linear/parallel_pixel/parallel_row/parallel_column/stream> <queue/classic>\n"

enum effect parse_effect(const char *str)
{
    if (strcmp(str, "blur") == 0)
    {
        return BLUR;
    }

    if (strcmp(str, "motion_blur") == 0)
    {
        return MOTION_BLUR;
    }

    if (strcmp(str, "find_edges") == 0)
    {
        return FIND_EDGES;
    }

    if (strcmp(str, "emboss") == 0)
    {
        return EMBOSS;
    }

    return -1;
}

enum algorithm_type parse_algorithm_type(const char *str)
{
    if (strcmp(str, "linear") == 0)
    {
        return LINEAR;
    }

    if (strcmp(str, "parallel_pixel") == 0)
    {
        return PARALLEL_PIXEL;
    }

    if (strcmp(str, "parallel_row") == 0)
    {
        return PARALLEL_PIXEL;
    }

    if (strcmp(str, "parallel_column") == 0)
    {
        return PARALLEL_PIXEL;
    }

    if (strcmp(str, "stream") == 0)
    {
        return STREAM;
    }

    return -1;
}

enum strength parse_effect_strength(const char *str)
{
    if (strcmp(str, "1") == 0)
    {
        return SMALL;
    }

    if (strcmp(str, "2") == 0)
    {
        return MEDIUM;
    }

    if (strcmp(str, "3") == 0)
    {
        return BIG;
    }

    return -1;
}

enum type parse_type(const char *type)
{
    if (strcmp(type, "classic") == 0)
    {
        return CLASSIC;
    }

    if (strcmp(type, "queue") == 0)
    {
        return QUEUE;
    }
    return -1;
}

int has_suffix(const char *str, const char *suffix)
{
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
    {
        return 0;
    }
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char **parse_source_images(char ***argv, int *count)
{
    int capacity = 8;
    int n = 0;
    char **files = malloc(capacity * sizeof(char *));
    if (!files)
    {
        return NULL;
    }

    while (**argv && has_suffix(**argv, ".bmp"))
    {
        if (n >= capacity)
        {
            capacity *= 2;
            files = realloc(files, capacity * sizeof(char *));
            if (!files)
            {
                return NULL;
            }
        }
        files[n++] = **argv;
        (*argv)++;
    }

    *count = n;
    return files;
}

input_data *validate_input(int argc, char **argv)
{
    input_data *input = malloc(sizeof(*input));
    if (!input)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }
    int img_count = 0;
    argv++;
    char **images = parse_source_images(&argv, &img_count);

    if (argc - img_count < SETTINGS_NUMBER)
    {
        error(USAGE_MSG);
        return NULL;
    };

    input->src_images = images;
    input->images_number = img_count;
    int effect = parse_effect(*argv);
    if (effect == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    input->effect_type = effect;

    int strength = parse_effect_strength(*(++argv));
    if (strength == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    input->effect_strength = strength;
    input->folder_for_store = *(++argv);
    int algorithm = parse_algorithm_type(*(++argv));
    if (algorithm == -1)
    {
        error(USAGE_MSG);
        return NULL;
    };
    input->algorithm = algorithm;
    int type = parse_type(*(++argv));
    if (type == -1)
    {
        error(USAGE_MSG);
        return NULL;
    }
    input->type = type;
    return input;
}
