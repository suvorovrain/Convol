#include "queue.h"
#include <stdatomic.h>
#include "../../filters/filters.h"

#ifndef SUBJECTS_H
#define SUBJECTS_H

typedef struct reader_parameters
{
    b_queue *queue_in;
    size_t readers_number;
    size_t workers_number;
    char **files_list;
    size_t files_number;
    atomic_size_t *next_file_id;
    atomic_int *next_task_id;
} rd_params_t;

typedef struct producer_parameters
{
    b_queue *queue_in;
    b_queue *queue_out;
    convolution_filter *filter;

} pr_params_t;

typedef struct consumer_parameters
{
    b_queue *queue_out;
    char *dest_folder;
} cm_params_t;

void *read(void *param);
void *produce(void *param);
void *consume(void *param);

#endif // SUBJECTS_H