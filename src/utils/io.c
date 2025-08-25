#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../vendor/stb_image_write.h"
#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


#define error(...)(fprintf(stderr, __VA_ARGS__))

int save_image(const char *dest, image_data *image)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s", dest);

    char *slash = strrchr(path, '/');
    if (slash) {
        *slash = '\0';
        if (mkdir(path,0777) != 0) {
              if (errno != EEXIST) {
                error("ERROR: cannot create directory\n");
                return 1;
            }
        }
        *slash = '/';
    }

    int res = stbi_write_bmp(dest, image->width, image->height,
                             image->components, image->image);
    if (res == 0) {
        error("ERROR: image save failed\n");
        return 1;
    }
    return 0;
}


image_data *load_image(char *path)
{
    int x, y, n;
    unsigned char *image = stbi_load(path, &x, &y, &n, 0);
    if (!image)
    {
        error("ERROR: image read failed");
        return NULL;
    };
    image_data *image_data = malloc(sizeof(*image_data));
    if (!image_data)
    {
        stbi_image_free(image);
        error("ERROR: malloc failed\n");
        return NULL;
    }
    image_data->image = image;
    image_data->height = y;
    image_data->width = x;
    image_data->components = n;
    return image_data;
}