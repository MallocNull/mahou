#include "queue.h"

queue_t* queue_init() {
    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
    queue->front = NULL;
    queue->back = NULL;
    
    return queue;
}

static queue_node_t* queue_node_init() {
    queue_node_t *node = (queue_node_t*)malloc(sizeof(queue_node_t));
    node->next = NULL;
    node->data = NULL;
    
    return node;
}

void queue_push(queue_t *queue, void *data) {
    queue_node_t *node = queue_node_init();
    node->data = data;
    
    if(queue->front == NULL)
        queue->front = queue->back = node;
    else
        queue->back = queue->back->next = node;
}

void* queue_pop(queue_t *queue) {
    if(queue_can_pop(queue) == FALSE)
        return NULL;
    
    queue_node_t *this = queue->front;
    void *data = this->data;
    if(queue->front == queue->back)
        queue->front = queue->back = NULL;
    else
        queue->front = queue->front->next;
    free(this);
    
    return data;
}

BOOL queue_can_pop(queue_t *queue) {
    return queue->front != NULL;
}

int queue_depth(queue_t *queue) {
    queue_node_t *ptr = queue->front;
    int i;
    
    for(i = 0; ptr != NULL; ++i)
        ptr = ptr->next;
    
    return i;
}

void queue_free(queue_t *queue) {
    queue_node_t *ptr = queue->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    
    free(queue);
}

void queue_nodes_free(queue_t *queue) {
    queue_node_t *ptr = queue->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr->data);
        
        free(ptr);
        ptr = next;
    }
    
    free(queue);
}

void queue_nodes_func_free(queue_t *queue, queueffptr ffptr) {
    queue_node_t *ptr = queue->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        if(ptr->data != NULL)
            (*ffptr)(ptr->data);
        
        free(ptr);
        ptr = next;
    }
    
    free(queue);
}
