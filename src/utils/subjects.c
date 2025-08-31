#include "include/subjects.h"
#include <stdlib.h>
void *read(void *param)
{
    // init params
    rd_pararms *rd_params = (rd_pararms *)param;
    size_t readers_number = rd_params->readers_number;
    size_t workers_number = rd_params->workers_number;
    char **files_list = rd_params->files_list;
    size_t files_number = rd_params->files_number;
    atomic_size_t *next_file_id = rd_params->next_file_id;
    atomic_int *next_id = rd_params->next_id;

    // read file
    
    // create task

    // put task in queue

    // pills
    for (;;)
    {
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