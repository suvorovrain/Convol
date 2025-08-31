#include "filters/filters.h"
#include "utils/include/io.h"
#include "utils/include/parce.h"
#include "utils/include/queue.h"
#include <stdio.h>
#include <stdlib.h>

#define READERS_NUMBER 2
#define WRITERS_NUMBER 4
#define WORKERS_NUMBER 2
#define IN_QUEUE_SIZE 4 * WORKERS_NUMBER
#define OUT_QUEUE_SIZE 2 * WORKERS_NUMBER

//----------------------------------------------------------------------------------------------
// Usage: ./convol <src_image> <blur/motion_blur/find_edges/emboss> <1/2/3> <dest>
// <linear/parallel_pixel/parallel_row/parallel_column/stream>
//----------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    input_data *input;
    input = validate_input(argc, argv);
    if (input == NULL)
    {
        return -1;
    };

    convolution_filter *filter = set_filter(input->effect_type, input->effect_strength);
    if (!filter)
    {
        return -1;
    };

    int res = 0;
    if (input->algorithm == STREAM)
    {
        res = stream_convolution(input, filter);
    }
    else
    {
        res = single_convolution(input, filter);
    }
    if (!res)
    {
        return -1;
    }

    return 0;
}

int stream_convolution(input_data *input, convolution_filter *filter)
{
    // init queues
    b_queue *in_queue = bq_init(IN_QUEUE_SIZE);
    b_queue *out_queue = bq_init(OUT_QUEUE_SIZE);

    // get files list
    char **names = NULL;
    size_t image_number = 0;

    int res = list_dir_entries(input->src_image, &names, &image_number);
    if (res != 0) {
        return 1;
    }

    for (size_t i = 0; i < image_number; ++i) {
        printf("%s\n", names[i]);
        free(names[i]);
    }

    // init readers

    // init producers

    // init consumers

    // join readers

    // join producers

    // join consumers

    free(names);
    
    return 0;
}

int single_convolution(input_data *input, convolution_filter *filter)
{
    // load image
    image_data *src_image = load_image(input->src_image);
    if (!src_image)
    {
        return -1;
    }

    // convolution
    image_data *result_image = convolution(src_image, filter, input->algorithm);
    if (!result_image)
    {
        return -1;
    }

    // save file
    int res = save_image(input->folder_for_store, result_image);
    if (!res)
    {
        return -1;
    }
    return 0;
}
