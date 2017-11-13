#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "common.h"

typedef void(*listfreefptr)(void*);
typedef BOOL(*listfindfptr)(void*);

typedef struct list_node_t list_node_t;
struct list_node_t {
    list_node_t *next;
    void *data;
};

typedef struct list_t list_t;
struct list_t {
    list_node_t *front;
    list_node_t *back;
    int size;

    list_node_t *iterator;
    list_node_t *iter_prev;
    int iter_pos;
};

list_t* list_init(void);

void list_append(list_t*, void*);
void list_prepend(list_t*, void*);
void list_insert(list_t*, void*, int);

int   list_size(list_t*);
void* list_get(list_t*, int);
void* list_front(list_t*);
void* list_back(list_t*);
int   list_find(list_t*, void*);
int   list_func_find(list_t*, listfindfptr);

void* list_remove(list_t*, int);
void* list_remove_front(list_t*);
void* list_remove_back(list_t*);
void  list_remove_item(list_t*, void*);

void  list_iter_reset(list_t*);
void* list_iter_next(list_t*);
void* list_iter_get(list_t*);
void* list_iter_remove(list_t*);
void  list_iter_insert_before(list_t*, void*);
void  list_iter_insert_after(list_t*, void*);
int   list_iter_pos(list_t*);

void list_free(list_t*);
void list_nodes_free(list_t*);
void list_nodes_func_free(list_t*, listfreefptr);

#endif
