#include <stdio.h>

#ifndef PARSE_H
#define PARSE_H

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define SETTINGS_NUMBER 6
enum effect {
    BLUR = 1,
    MOTION_BLUR = 2,
    FIND_EDGES = 3,
    EMBOSS = 4,
};

enum type
{
    QUEUE = 0,
    CLASSIC = 1,
};

//-------------------------------------------------------------------------------
// defines the strengh of applied effect
//-------------------------------------------------------------------------------

enum strength
{
    SMALL = 1,
    MEDIUM = 2,
    BIG = 3,
};

enum algorithm_type
{
    LINEAR = 1,
    PARALLEL_PIXEL = 2,
    PARALLEL_ROW = 3,
    PARALLEL_COLUMN = 4,
    STREAM = 5,
};

typedef struct
{
    char **src_images;
    size_t images_number;
    enum effect effect_type;
    enum strength effect_strength;
    char *folder_for_store;
    enum algorithm_type algorithm;
    enum type type;
} input_data;

input_data *validate_input(int argc, char **argv);
#endif // PARCE_H
