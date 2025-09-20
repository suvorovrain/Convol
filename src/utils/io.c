#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../vendor/stb_image_write.h"
#include "include/io.h"

int save_image(const char *dest, image_data *image)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s", dest);

    char *slash = strrchr(path, '/');
    if (slash)
    {
        *slash = '\0';
        if (mkdir(path, 0777) != 0)
        {
            if (errno != EEXIST)
            {
                send_error("ERROR: cannot create directory\n");
                return 1;
            }
        }
        *slash = '/';
    }

    int res = stbi_write_bmp(dest, image->width, image->height, image->components, image->image);
    if (res == 0)
    {
        send_error("ERROR: image save failed\n");
        return 1;
    };
    return 0;
}

image_data *load_image(char *path)
{
    int x, y, n;
    unsigned char *image = stbi_load(path, &x, &y, &n, 0);
    if (!image)
    {
        send_error("ERROR: image read failed\n");
        return NULL;
    };
    image_data *image_data = malloc(sizeof(*image_data));
    if (!image_data)
    {
        stbi_image_free(image);
        send_error("ERROR: malloc failed\n");
        return NULL;
    }
    image_data->image = image;
    image_data->height = y;
    image_data->width = x;
    image_data->components = n;
    return image_data;
}

char *path_trim(const char *path)
{
    if (!path)
    {
        return NULL;
    }

    char *tmp = strdup(path);
    if (!tmp)
    {
        return NULL;
    }

    char *trimmed_name = basename(tmp);
    if (!trimmed_name)
    {
        return NULL;
    }
    char *result = strdup(trimmed_name);

    free(tmp);
    return result;
}

char *path_join(const char *dir, const char *file)
{
    if (!dir || !*dir)
    {
        if (!file)
        {
            return strdup("");
        }
        return strdup(file);
    }
    if (!file || !*file)
    {
        return strdup(dir);
    }

    size_t nd = strlen(dir), nf = strlen(file);
    int need_sep = dir[nd - 1] != '/';

    size_t total = nd + (need_sep ? 1 : 0) + nf;
    char *out = (char *)malloc(total + 1);
    if (!out)
    {
        send_error("ERROR: malloc failed\n");
        return NULL;
    }

    memcpy(out, dir, nd);
    size_t pos = nd;
    if (need_sep)
    {
        out[pos++] = '/';
    }

    memcpy(out + pos, file, nf);
    out[total] = '\0';
    return out;
}

char *add_suffix(const char *image_name)
{
    const char *dot = strrchr(image_name, '.');
    size_t base_len = (size_t)(dot - image_name);
    const char *ext = dot;
    const char *suffix = "_edited";
    size_t new_len = base_len + strlen(suffix) + strlen(ext);
    char *result = malloc(new_len + 1);
    if (!result)
    {
        send_error("ERROR: malloc failed\n");
        return NULL;
    }
    snprintf(result, new_len + 1, "%.*s%s%s", (int)base_len, image_name, suffix, ext);

    return result;
}

void free_image(image_data *image_data)
{
    if (!image_data)
    {
        return;
    }
    free(image_data->image);
    free(image_data);
}
