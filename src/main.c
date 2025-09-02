#include "filters/filters.h"
#include "utils/include/io.h"
#include "utils/include/parce.h"
#include "utils/include/queue.h"
#include "utils/include/subjects.h"
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
int stream_convolution(input_data *input, convolution_filter *filter)
{
    // init queues
    b_queue *in_queue = bq_init(IN_QUEUE_SIZE);
    b_queue *out_queue = bq_init(OUT_QUEUE_SIZE);

    // get files list
    char **names = NULL;
    size_t image_number = 0;

    int res = list_dir_entries(input->src_image, &names, &image_number);
    if (res != 0)
    {
        return 1;
    }

    // for (size_t i = 0; i < image_number; ++i) {
    //     printf("%s\n", names[i]);
    //     char *trim_name = path_trim(names[i]);
    //     printf("%s\n", trim_name);
    //     char *new_name = path_join(input->folder_for_store,trim_name);
    //     printf("%s\n", new_name);
    //     free(names[i]);
    // }

    // init readers
    atomic_size_t file_index;
    atomic_int task_index;

    atomic_init(&file_index, 0);
    atomic_init(&task_index, 0);

    pthread_t readers[READERS_NUMBER];
    rd_params_t rd_params[READERS_NUMBER];
    for (int i = 0; i < READERS_NUMBER; i++)
    {
        rd_params[i].files_list = names;
        rd_params[i].files_number = image_number;
        rd_params[i].next_file_id = &file_index;
        rd_params[i].next_task_id = &task_index;
        rd_params[i].queue_in = in_queue;
        pthread_create(readers + i, NULL, read, rd_params + i);
    }

    // init producers
    pthread_t producers[WORKERS_NUMBER];
    pr_params_t pr_params[WORKERS_NUMBER];

    for (int i = 0; i < WORKERS_NUMBER; i++)
    {
        pr_params[i].filter = filter;
        pr_params[i].queue_in = in_queue;
        pr_params[i].queue_out = out_queue;
        pthread_create(producers + i, NULL, produce, pr_params);
    }
    // init consumers
    pthread_t consumers[WRITERS_NUMBER];
    cm_params_t cm_params[WRITERS_NUMBER];
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        cm_params[i].dest_folder = input->folder_for_store;
        cm_params[i].queue_out = out_queue;
        pthread_create(consumers + i, NULL, consume, cm_params);
    }

    // join readers
    for (int i = 0; i < READERS_NUMBER; i++)
    {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < WORKERS_NUMBER; i++)
    {
        in_task *task = create_in_task(PILL_TASK_ID, NULL, NULL);
        bq_enqueue(in_queue, task);
    }
    // join producers
    for (int i = 0; i < WORKERS_NUMBER; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        out_task *task = create_out_task(PILL_TASK_ID, NULL, NULL);
        bq_enqueue(out_queue, task);
    }
    // join consumers
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        pthread_join(consumers[i], NULL);
    }
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
