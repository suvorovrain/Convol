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
        size_t file_id = atomic_fetch_add_explicit(next_file_id, 1, memory_order_relaxed);
        if (file_id >= files_number)
        {
            break;
        }
        // read file
        char *filename = files_list[file_id];

        image_data *src_image = load_image(filename);
        if (!src_image)
        {
            // TODO
            assert(false);
        }

        // create task
        int task_id = atomic_fetch_add_explicit(next_task_id, 1, memory_order_relaxed);
        in_task *task = create_in_task(task_id, src_image, filename);
        if (!task)
        {
            // TODO
            assert(false);
        }

        // put task in queue
        bq_enqueue(queue_in, task);
    }

    return NULL;
}

void *produce(void *param)
{
    // init params
    pr_params_t *pr_params = (pr_params_t *)param;
    b_queue *queue_in = pr_params->queue_in;
    b_queue *queue_out = pr_params->queue_out;
    convolution_filter *filter = pr_params->filter;
    for (;;)
    {
        // get task
        in_task *in_task = bq_dequeue(queue_in);
        if (in_task->id == -1)
        {
            break;
        }
        // convolution
        image_data *result_image = linear_convolution(in_task->src_image, filter);

        // create task
        out_task *out_task = create_out_task(in_task->id, result_image, in_task->image_name);

        // put task
        bq_enqueue(queue_out, out_task);
    }
    return NULL;
}

void *consume(void *param)
{
    // init params
    cm_params_t *cm_params = (cm_params_t *)param;
    b_queue *queue_out = cm_params->queue_out;
    char *dest_folder = cm_params->dest_folder;
    for (;;)
    {
        // get task
        out_task *out_task = bq_dequeue(queue_out);
        if (out_task->id == -1)
        {
            break;
        }
        // save picture
        char *dest_dir = path_join(dest_folder, add_suffix(path_trim(out_task->image_name)));

        int res = save_image(dest_dir, out_task->result_image);
        // TODO: error handling
        // if (!res)
        // {
        //     return -1;
        // }
    }
    return NULL;
}