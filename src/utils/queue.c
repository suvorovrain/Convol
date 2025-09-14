#include "include/queue.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

int bq_enqueue(b_queue *queue, void *item)
{
    node *element = malloc(sizeof *element);
    if (!element)
    {
        return -1;
    }
    element->value = item;
    element->next = NULL;

    pthread_mutex_lock(&queue->lock);
    while (queue->size == queue->capacity)
    {
        pthread_cond_wait(&queue->not_full_cond, &queue->lock);
    }

    queue->tail->next = element;
    queue->tail = element;
    queue->size++;

    pthread_cond_signal(&queue->not_empty_cond);
    pthread_mutex_unlock(&queue->lock);
    return 0;
    ;
}

void *bq_dequeue(b_queue *queue)
{
    pthread_mutex_lock(&queue->lock);
    while (queue->head->next == NULL)
    {
        pthread_cond_wait(&queue->not_empty_cond, &queue->lock);
    }

    node *old = queue->head;
    node *n = old->next;
    void *res = n->value;

    queue->head = n;
    queue->size--;

    pthread_mutex_unlock(&queue->lock);
    free(old);

    pthread_mutex_lock(&queue->lock);
    pthread_cond_signal(&queue->not_full_cond);
    pthread_mutex_unlock(&queue->lock);

    return res;
}

b_queue *bq_init(size_t capacity)
{
    if (capacity == 0)
    {
        return NULL;
    }

    b_queue *queue = calloc(1, sizeof(*queue));
    if (!queue)
    {
        error("Error: malloc failed\n");
        return NULL;
    };

    node *sentinel = malloc(sizeof(*sentinel));
    if (!sentinel)
    {
        error("Error: malloc failed\n");
        free(queue);
        return NULL;
    };
    sentinel->value = NULL;
    sentinel->next = NULL;
    queue->head = queue->tail = sentinel;

    atomic_init(&(queue->size), 0);

    int init_result = pthread_mutex_init(&(queue->lock), NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: mutex initialization failed\n");
        return NULL;
    };

    init_result = pthread_cond_init(&(queue->not_empty_cond), NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: condition variable initialization failed\n");
        return NULL;
    };
    init_result = pthread_cond_init(&(queue->not_full_cond), NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: condition variable initialization failed\n");
        return NULL;
    };
    queue->capacity = capacity;
    return queue;
}

void bq_destroy(b_queue *queue)
{
    if (!queue)
    {
        return;
    };
    free(queue->head);
    pthread_cond_destroy(&(queue->not_empty_cond));
    pthread_cond_destroy(&(queue->not_full_cond));
    pthread_mutex_destroy(&(queue->lock));
    free(queue);
}

in_task *create_in_task(int id, image_data *image, char *filename)
{
    in_task *task = malloc(sizeof(*task));
    if (!task)
    {
        error("ERROR: malloc failed");
        return NULL;
    }
    task->id = id;
    task->src_image = image;
    task->image_name = filename;
    return task;
}

out_task *create_out_task(int id, image_data *image, char *filename)
{
    out_task *task = malloc(sizeof(*task));
    if (!task)
    {
        error("ERROR: malloc failed");
        return NULL;
    }
    task->id = id;
    task->result_image = image;
    task->image_name = filename;
    return task;
}
