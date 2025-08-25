#include "filters/filters.h"
#include "utils/io.h"
#include "utils/parce.h"
#include <stdlib.h>
#include <stdio.h>


//----------------------------------------------------------------------------------------------
// Usage: ./convol <src_image> <blur/motion_blur/find_edges/sharpen> <1/2/3> <dest>
// <linear/parallel_pixel/parallel_row/parallel_column/split>
//----------------------------------------------------------------------------------------------


int main(int argc, char **argv)
{
    input_data *input;
    input = validate_input(argc, argv);
    if (input == NULL)
    {
        return -1;
    };
    printf("after validate\n");
    printf("Input file: %s\n", input->src_image);
    printf("Effect type: %d\n", input->effect_type);
    printf("Effect strength: %d\n", input->effect_strength);
    printf("Destination folder: %s\n", input->folder_for_store);
    printf("Method: %d\n", input->algorithm);
    convolution_filter *filter =
        set_filter(input->effect_type, input->effect_strength);
    if (!filter)
    {
        return -1;
    };

    // load image
    image_data *src_image = load_image(input->src_image);
    if (!src_image)
    {
        return -1;
    };

    // convolution
    image_data *result_image = convolution(src_image, filter,input->algorithm);
    if (!result_image)
    {
        return -1;
    };

    // save file
    int res = save_image(input->folder_for_store, result_image);
    if (!res)
    {
        return -1;
    };
    return 0;
}