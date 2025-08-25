typedef struct
{
    unsigned char *image;
    int height;
    int width;
    int components;
} image_data;

image_data *load_image(char *path);
int save_image(char *dest, image_data *image);
