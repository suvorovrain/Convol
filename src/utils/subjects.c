#include "include/subjects.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../algo/include/linear_convolution.h"

void *read(void *param)
{
    // init params
    rd_params_t *rd_params = (rd_params_t *)param;
    b_queue *queue_in = rd_params->queue_in;
    char **files_list = rd_params->files_list;
    size_t files_number = rd_params->files_number;
    atomic_size_t *next_file_id = rd_params->next_file_id;
    atomic_int *next_task_id = rd_params->next_task_id;

    for (;;)
    {
        // get filename
        size_t file_id = atomic_fetch_add_explicit(next_file_id, 1, memory_order_relaxed);
        if (file_id >= files_number)
        {
            break;
        }
        char *filename = files_list[file_id];

        // load file
        image_data *src_image = load_image(filename);
        if (!src_image)
        {
            break;
        }

        // create in_task
        int task_id = atomic_fetch_add_explicit(next_task_id, 1, memory_order_relaxed);
        in_task *task = create_in_task(task_id, src_image, filename);
        if (!task)
        {
            break;
        }

        // put in_task
        int res = bq_enqueue(queue_in, task);
        if (res)
        {
            break;
        }
    }

    return NULL;
}

void *work(void *param)
{
    // init params
    wk_params_t *wk_params = (wk_params_t *)param;
    b_queue *queue_in = wk_params->queue_in;
    b_queue *queue_out = wk_params->queue_out;
    convolution_func convolution = wk_params->convolution;
    convolution_filter *filter = wk_params->filter;
    for (;;)
    {
        // get in_task
        in_task *in_task = bq_dequeue(queue_in);
        if (in_task->id == -1)
        {
            break;
        }

        // convolution
        image_data *result_image = convolution(in_task->src_image, filter);
        if (!result_image)
        {
            break;
        }

        // create out_task
        out_task *out_task = create_out_task(in_task->id, result_image, in_task->image_name);
        if (!out_task)
        {
            break;
        }

        // put task
        bq_enqueue(queue_out, out_task);
    }
    return NULL;
}

void *write(void *param)
{
    // init params
    wr_params_t *wr_params = (wr_params_t *)param;
    b_queue *queue_out = wr_params->queue_out;
    char *dest_folder = wr_params->dest_folder;
    for (;;)
    {
        // get task
        out_task *out_task = bq_dequeue(queue_out);
        if (out_task->id == -1)
        {
            break;
        }

        // save picture
        char *result_path = path_join(dest_folder, add_suffix(path_trim(out_task->image_name)));
        int res = save_image(result_path, out_task->result_image);
        if (res)
        {
            break;
        }
    }
    return NULL;
}
