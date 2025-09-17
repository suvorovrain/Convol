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

void free_image(image_data *image);
int save_image(const char *dest, image_data *image);
char *path_trim(const char *path);
char *path_join(const char *dir, const char *file);
char *add_suffix(const char *image_name);

#endif // IO_H
