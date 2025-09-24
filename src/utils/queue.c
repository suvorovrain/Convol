#include "include/queue.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    b_queue *queue = calloc(1, sizeof *queue);
    if (!queue)
    {
        send_error("Error: malloc failed\n");
        return NULL;
    }

    node *sentinel = malloc(sizeof *sentinel);
    if (!sentinel)
    {
        send_error("Error: malloc failed\n");
        free(queue);
        return NULL;
    }
    sentinel->value = NULL;
    sentinel->next = NULL;
    queue->head = queue->tail = sentinel;
    atomic_init(&(queue->size), 0);

    int rc = pthread_mutex_init(&(queue->lock), NULL);
    if (rc)
    {
        send_error("Error: mutex init failed\n");
        free(sentinel);
        free(queue);
        return NULL;
    }
    rc = pthread_cond_init(&(queue->not_empty_cond), NULL);
    if (rc)
    {
        send_error("Error: cond init failed\n");
        pthread_mutex_destroy(&(queue->lock));
        free(sentinel);
        free(queue);
        return NULL;
    }
    rc = pthread_cond_init(&(queue->not_full_cond), NULL);
    if (rc)
    {
        send_error("Error: cond init failed\n");
        pthread_cond_destroy(&(queue->not_empty_cond));
        pthread_mutex_destroy(&(queue->lock));
        free(sentinel);
        free(queue);
        return NULL;
    }

    queue->capacity = capacity;
    return queue;
}

void bq_destroy(b_queue *queue)
{
    if (!queue)
    {
        return;
    }

    node *n = queue->head;
    while (n)
    {
        node *next = n->next;
        free(n);
        n = next;
    }

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
        send_error("ERROR: malloc failed\n");
        return NULL;
    }
    task->id = id;
    task->src_image = image;
    task->image_name = filename;
    return task;
}

out_task *create_out_task(int id, image_data *image, char *filename)
{
    out_task *task = malloc(sizeof *task);
    if (!task)
    {
        send_error("ERROR: malloc failed\n");
        return NULL;
    }

    task->id = id;
    task->result_image = image;
    task->image_name = filename ? strdup(filename) : NULL;
    if (filename && !task->image_name)
    {
        free(task);
        return NULL;
    }

    return task;
}

void free_in_task(in_task *task)
{
    if (!task)
    {
        return;
    }

    if (task->src_image)
    {
        free_image(task->src_image);
        task->src_image = NULL;
    }
    task->image_name = NULL;
    free(task);
}

void free_out_task(out_task *task)
{
    if (!task)
    {
        return;
    }

    if (task->result_image)
    {
        free_image(task->result_image);
        task->result_image = NULL;
    }
    free(task->image_name);
    task->image_name = NULL;
    free(task);
}
