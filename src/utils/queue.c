#include "include/queue.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
void bq_enqueue(b_queue *queue, void *item)
{
    bool must_wake_dequeuers = false;
    node *element = malloc(sizeof(node));
    if (!element)
    {
        // TODO: handle
        return;
    };
    element->value = item;
    element->next = NULL;

    pthread_mutex_lock(&(queue->enq_lock));
    while (atomic_load_explicit(&queue->size, memory_order_relaxed) == queue->capacity)
    {
        pthread_cond_wait(&(queue->not_full_cond), &(queue->enq_lock));
    }
    queue->tail->next = element;
    queue->tail = element;
    size_t prev = atomic_fetch_add_explicit(&(queue->size), 1, memory_order_relaxed);
    if (prev == 0)
    {
        must_wake_dequeuers = true;
    }
    pthread_mutex_unlock(&(queue->enq_lock));
    if (must_wake_dequeuers)
    {
        pthread_mutex_lock(&(queue->deq_lock));
        pthread_cond_broadcast(&(queue->not_empty_cond));
        pthread_mutex_unlock(&(queue->deq_lock));
    };
}

void *bq_dequeue(b_queue *queue)
{
    void *result;
    bool must_wake_enqueuers = false;
    node *old_head, *n;

    pthread_mutex_lock(&(queue->deq_lock));
    while (queue->head->next == NULL)
    {
        pthread_cond_wait(&(queue->not_empty_cond), &(queue->deq_lock));
    }

    old_head = queue->head;
    n = old_head->next;
    result = n->value;
    n->value = NULL;
    queue->head = n;

    size_t prev = atomic_fetch_add_explicit(&(queue->size), -1, memory_order_relaxed);
    if (prev == queue->capacity)
    {
        must_wake_enqueuers = true;
    }
    pthread_mutex_unlock(&(queue->deq_lock));
    free(old_head);
    if (must_wake_enqueuers)
    {
        pthread_mutex_lock(&(queue->enq_lock));
        pthread_cond_broadcast(&(queue->not_full_cond));
        pthread_mutex_unlock(&(queue->enq_lock));
    };
    return result;
}

b_queue *bq_init(size_t capacity)
{
    if (capacity==0){
        return NULL;
    }

    b_queue *queue = calloc(1,sizeof (*queue));
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
    sentinel->value=NULL;
    sentinel->next=NULL;
    queue->head=queue->tail=sentinel;

    atomic_init(&(queue->size),0);

    int init_result = pthread_mutex_init(&(queue->enq_lock), NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: mutex initialization failed\n");
        return NULL;
    };
    init_result = pthread_mutex_init(&(queue->deq_lock), NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: mutex initialization failed\n");
        return NULL;
    };

    init_result = pthread_cond_init(&(queue->not_empty_cond),NULL);
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: condition variable initialization failed\n");
        return NULL;
    };
    init_result = pthread_cond_init(&(queue->not_full_cond),NULL); 
    if (init_result != 0)
    {
        free(queue);
        free(sentinel);
        error("Error: condition variable initialization failed\n");
        return NULL;
    };
    queue->capacity=capacity;
    return queue;
}

void bq_destroy(b_queue *queue) {
    if (!queue){
        return;
    };
    free(queue->head);
    pthread_cond_destroy(&(queue->not_empty_cond));
    pthread_cond_destroy(&(queue->not_full_cond));
    pthread_mutex_destroy(&(queue->deq_lock));
    pthread_mutex_destroy(&(queue->enq_lock));
    free(queue);
}

in_task *create_in_task(int id, image_data *image){
    in_task *task = malloc(sizeof(*task));
    if (!task){
        return NULL;
    }
    task->id=id;
    task->src_image=image;
    return task;
}