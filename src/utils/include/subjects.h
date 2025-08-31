#include "queue.h"
#include <stdatomic.h>

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
} rd_pararms;

typedef struct producer_parameters
{
} pr_params;

typedef struct consumer_parameters
{
} cm_params;

void *read(void *param);
void *produce(void *param);
void *consume(void *param);

#endif // SUBJECTS_H