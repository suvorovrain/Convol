#include "include/subjects.h"
#include <stdbool.h>
#include <stdlib.h>
void *read(void *param)
{
    // init params
    rd_pararms *rd_params = (rd_pararms *)param;
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
        in_task *task = create_in_task(task_id, src_image);
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
    for (;;)
    {
    }
    return NULL;
}

void *consume(void *param)
{
    for (;;)
    {
    }
    return NULL;
}