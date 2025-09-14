#include "../../algo/include/algo.h"
#include "../../filters/filters.h"
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
} rd_params_t;

typedef struct worker_parameters
{
    convolution_func convolution;
    b_queue *queue_in;
    b_queue *queue_out;
    convolution_filter *filter;
} wk_params_t;

typedef struct writer_parameters
{
    b_queue *queue_out;
    char *dest_folder;
} wr_params_t;

void *read(void *param);
void *work(void *param);
void *write(void *param);

#endif // SUBJECTS_H