#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "common.h"

typedef void(*queueffptr)(void*);

typedef struct queue_node_t queue_node_t;
struct queue_node_t {
    queue_node_t *next;
    void *data;
};

typedef struct queue_t queue_t;
struct queue_t {
    queue_node_t *front;
    queue_node_t *back;
};

queue_t* queue_init();

void queue_push(queue_t*, void*);
void* queue_pop(queue_t*);
BOOL queue_can_pop(queue_t*);
int queue_depth(queue_t*);

void queue_free(queue_t*);
void queue_nodes_free(queue_t*);
void queue_nodes_func_free(queue_t*, queueffptr);

#endif