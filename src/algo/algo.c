#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../filters/filters.h"
#include "../utils/include/io.h"
#include "../utils/include/parse.h"
#include "../utils/include/subjects.h"
#include "include/algo.h"
#include "include/linear_convolution.h"
#include "include/parallel_convolution.h"

convolution_func get_convolution(int type)
{
    switch (type)
    {
    case LINEAR:
        return linear_convolution;
    case PARALLEL_PIXEL:
        return parallel_pixel_convolution;
    case PARALLEL_ROW:
        return parallel_row_convolution;
    case PARALLEL_COLUMN:
        return parallel_column_convolution;
    default:
        error("ERROR: unsupported algorithm type\n");
        return NULL;
    }
}

image_data *create_canvas(image_data *image)
{
    image_data *out = malloc(sizeof(*out));
    if (!out)
    {
        error("ERROR: malloc failed\n");
        return NULL;
    }

    out->width = image->width;
    out->height = image->height;
    out->components = image->components;

    size_t size = (size_t)out->width * out->height * out->components;
    out->image = malloc(size);
    if (!out->image)
    {
        error("ERROR: malloc failed\n");
        free(out);
        return NULL;
    };
    return out;
}

int queue_convolution(input_data *input, convolution_filter *filter)
{
    // init queues
    b_queue *in_queue = bq_init(IN_QUEUE_SIZE);
    b_queue *out_queue = bq_init(OUT_QUEUE_SIZE);

    // get files list
    char **src_images = input->src_images;
    size_t image_number = input->images_number;

    // init readers
    atomic_size_t file_index;
    atomic_int task_index;

    atomic_init(&file_index, 0);
    atomic_init(&task_index, 0);

    pthread_t readers[READERS_NUMBER];
    rd_params_t rd_params[READERS_NUMBER];
    for (int i = 0; i < READERS_NUMBER; i++)
    {
        rd_params[i].files_list = src_images;
        rd_params[i].files_number = image_number;
        rd_params[i].next_file_id = &file_index;
        rd_params[i].next_task_id = &task_index;
        rd_params[i].queue_in = in_queue;
        pthread_create(readers + i, NULL, read, rd_params + i);
    }

    // init producers
    pthread_t workers[WORKERS_NUMBER];
    wk_params_t wk_params[WORKERS_NUMBER];

    convolution_func func = get_convolution(input->algorithm);
    if (!func)
    {
        return -1;
    }

    for (int i = 0; i < WORKERS_NUMBER; i++)
    {
        wk_params[i].filter = filter;
        wk_params[i].queue_in = in_queue;
        wk_params[i].queue_out = out_queue;
        wk_params[i].convolution = func;
        pthread_create(workers + i, NULL, work, wk_params);
    }
    // init writers
    pthread_t writers[WRITERS_NUMBER];
    wr_params_t wr_params[WRITERS_NUMBER];
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        wr_params[i].dest_folder = input->folder_for_store;
        wr_params[i].queue_out = out_queue;
        pthread_create(writers + i, NULL, write, wr_params);
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
        pthread_join(workers[i], NULL);
    }
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        out_task *task = create_out_task(PILL_TASK_ID, NULL, NULL);
        bq_enqueue(out_queue, task);
    }
    // join consumers
    for (int i = 0; i < WRITERS_NUMBER; i++)
    {
        pthread_join(writers[i], NULL);
    }
    free(src_images);

    return 0;
}

int classic_convolution(input_data *input, convolution_filter *filter)
{
    char **src_images = input->src_images;
    for (size_t i = 0; i < input->images_number; i++)
    {
        // load image
        image_data *src_image = load_image(src_images[i]);
        if (!src_image)
        {
            return -1;
        }

        // convolution
        convolution_func func = get_convolution(input->algorithm);
        if (!func)
        {
            return -1;
        }

        image_data *result_image = func(src_image, filter);
        if (!result_image)
        {
            return -1;
        }

        // save file
        char *dest_dir = path_join(input->folder_for_store, add_suffix(path_trim(src_images[i])));

        int res = save_image(dest_dir, result_image);
        if (res)
        {
            return -1;
        }
    }
    return 0;
}
