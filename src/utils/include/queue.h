#include "io.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

#ifndef QUEUE_H
#define QUEUE_H

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef struct node
{
    void *value;
    struct node *next;
} node;

typedef struct b_queue
{
    pthread_mutex_t enq_lock, deq_lock;
    pthread_cond_t not_empty_cond, not_full_cond;
    atomic_size_t size;
    node *head, *tail;
    size_t capacity;
} b_queue;

void bq_enqueue(b_queue *q, void *item);
void *bq_dequeue(b_queue *q);
b_queue *bq_init(size_t capacity);
void bq_destroy(b_queue *q);

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

#endif // QUEUE_H