#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../vendor/stb_image_write.h"
#include "include/io.h"
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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
                error("ERROR: cannot create directory\n");
                return 1;
            }
        }
        *slash = '/';
    }

    int res = stbi_write_bmp(dest, image->width, image->height, image->components, image->image);
    if (res == 0)
    {
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

int list_dir_entries(const char *dirpath, char ***out, size_t *count)
{
    if (!dirpath || !out || !count)
    {
        return 1;
    }

    *out = NULL;
    *count = 0;

    DIR *src_dir = opendir(dirpath);
    if (!src_dir)
    {
        return 1;
    }

    size_t dl = strlen(dirpath);
    int need_slash = (dl > 0 && dirpath[dl - 1] != '/');

    char **arr = NULL;
    size_t n = 0, cap = 0;

    struct dirent *de;
    while ((de = readdir(src_dir)) != NULL)
    {
        const char *name = de->d_name;
        if ((name[0] == '.' && name[1] == '\0') ||
            (name[0] == '.' && name[1] == '.' && name[2] == '\0'))
        {
            continue;
        }

        size_t nl = strlen(name);
        size_t plen = dl + need_slash + nl + 1;

        char *path = (char *)malloc(plen);
        if (!path)
        {
            closedir(src_dir);
            for (size_t i = 0; i < n; ++i)
            {
                free(arr[i]);
            }
            free(arr);
            return 1;
        }

        memcpy(path, dirpath, dl);
        if (need_slash)
        {
            path[dl] = '/';
        }
        memcpy(path + dl + need_slash, name, nl + 1);

        if (n == cap)
        {
            size_t newcap = cap ? cap * 2 : 32;
            char **tmp = (char **)realloc(arr, newcap * sizeof(char *));
            if (!tmp)
            {
                free(path);
                closedir(src_dir);
                for (size_t i = 0; i < n; ++i)
                {
                    free(arr[i]);
                }
                free(arr);
                return 1;
            }
            arr = tmp;
            cap = newcap;
        }

        arr[n++] = path;
    }

    closedir(src_dir);
    *out = arr;
    *count = n;
    return 0;
}

// TODO: free
char *path_trim(const char *path)
{
    if (!path){
        return NULL;
    }
        
    char *tmp = strdup(path);
    if (!tmp){
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