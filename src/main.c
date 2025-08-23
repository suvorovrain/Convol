#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"
#include "filters/filters.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef struct
{
    char *src_image;
    char *effect_type;
    int effect_strength;
    char *folder_for_store;
    char *algorithm;
} input_data;

typedef struct {
    unsigned char *image;
    int height;
    int width;
    int components;
} image_data;

//----------------------------------------------------------------------------------------------
// Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen> <1/2/3/4/5/> <dest>
// <linear/parallel/split>
//----------------------------------------------------------------------------------------------

input_data *validate_input(int argc, char **argv) 
{
    if (argc != 6)
    {
        return NULL;
    };
    input_data *input = malloc(sizeof(*input));
    if (!input) {
        error("ERROR: malloc failed\n");
        return NULL;
    }

    input->src_image=argv[1];
    input->effect_type=argv[2];
    input->effect_strength=atoi(argv[3]);
    input->folder_for_store=argv[4];
    input->algorithm=argv[5];
    return input;
}

save_result(char *dest, image_data *image) {
    return stbi_write_bmp(dest, image->width,  image->height, image->components, image->image);
}

image_data *load_image(char *path) {
    int x,y,n;
    unsigned char *image = stbi_load(path,&x,&y,&n,0);
    if (image = NULL){
        error("ERROR: image read failed");
        return NULL;
    };
    image_data *image_data = malloc(sizeof(*image));
    if (image==NULL) {
        error("ERROR: malloc failed\n");
        return NULL;
    }
    image_data->image=image;
    image_data->height=y;
    image_data->width=x;
    image_data->components;
    return image_data
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
    return 0;
}

convolution_filter *set_filter(enum effect eff, int strength)
{
    printf("set_filter\n");
    switch (eff)
    {
    case BLUR:
        return create_blur_convolution_filter(strength);
    case MOTION_BLUR:
        return create_motion_blur_convolution_filter(strength);
    case FIND_EDGES:
        return create_find_edges_convolution_filter(strength);
    case EMBOSS:
        return create_emboss_convolution_filter(strength);
    default:
        error("ERROR: invalid filter name");
    }
}

int main(int argc, char **argv)
{
    input_data *input;
    input = validate_input(argc, argv);
    if (input == NULL)
    {
        error("Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen/emboss> "
              "<1/2/3/4/5/> <dest> <linear/parallel/split>\n");
        return 1;
    };
    printf("after validate\n");
    printf("Input file: %s\n", input->src_image);
    printf("Effect type: %s\n",input->effect_type);
    printf("Effect strength: %d\n", input->effect_strength);
    printf("Destination folder: %s\n", input->folder_for_store);
    printf("Method: %s\n", input->algorithm);
    convolution_filter *filter = set_filter(parse_effect(input->effect_type), input->effect_strength);

    // load file

    // choose algorithm
    
    // save file

    return 0;
}