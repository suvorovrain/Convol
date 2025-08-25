
#include "io.h"
#include "stdlib.h"
int save_result(char *dest, image_data *image)
{
    int res = stbi_write_bmp(dest, image->width, image->height, image->components, image->image);
    if (res = 0) {
        error("ERROR: image save failed");
        return 1;
    };
    return 0;
}

image_data *load_image(char *path)
{
    int x, y, n;
    unsigned char *image = stbi_load(path, &x, &y, &n, 0);
    if (image = NULL)
    {
        error("ERROR: image read failed");
        return NULL;
    };
    image_data *image_data = malloc(sizeof(*image));
    if (image == NULL)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }
    image_data->image = image;
    image_data->height = y;
    image_data->width = x;
    image_data->components;
    return image_data;
}