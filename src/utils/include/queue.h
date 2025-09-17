#include "io.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

#ifndef QUEUE_H
#define QUEUE_H

#define error(...) (fprintf(stderr, __VA_ARGS__))
// used to tell the readers/workers/writets that there is no more tasks

#define PILL_TASK_ID -1
typedef struct node
{
    void *value;
    struct node *next;
} node;

typedef struct b_queue
{
    pthread_mutex_t lock;
    pthread_cond_t not_empty_cond, not_full_cond;
    atomic_size_t size;
    node *head, *tail;
    size_t capacity;
} b_queue;

int bq_enqueue(b_queue *queue, void *item);
void *bq_dequeue(b_queue *queue);
b_queue *bq_init(size_t capacity);
void bq_destroy(b_queue *queue);

typedef struct
{
    int id;
    image_data *src_image;
    char *image_name;
} in_task;
typedef struct
{
    int id;
    image_data *result_image;
    char *image_name;
} out_task;

in_task *create_in_task(int id, image_data *image, char *filename);
out_task *create_out_task(int id, image_data *image, char *filename);
void free_in_task(in_task *task);
void free_out_task(out_task *task);

#endif // QUEUE_H