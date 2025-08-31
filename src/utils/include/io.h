#include <stdio.h>
#ifndef IO_H
#define IO_H

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef struct
{
    unsigned char *image;
    int height;
    int width;
    int components;
} image_data;

image_data *load_image(char *path);
int save_image(const char *dest, image_data *image);
int list_dir_entries(const char *dirpath, char ***out, size_t *count);

#endif // IO_H
