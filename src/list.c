#include "list.h"

list_t* list_init() {
    list_t* list = (list_t*)malloc(sizeof(list_t));
    list->front = NULL;
    list->back = NULL;
    list->iterator = NULL;
    list->size = 0;

    return list;
}

static list_node_t* list_node_init() {
    list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
    node->next = NULL;
    node->data = NULL;

    return node;
}

static list_node_t* list_get_raw(list_t *list, int pos) {
    if(pos >= list->size)
        return NULL;

    int i = 0;
    list_node_t *ptr = list->front;
    for(i = 0; i < pos; ++i)
        ptr = ptr->next;

    return ptr;
}

static void list_add_first(list_t *list, void *data) {
    if(list->size != 0)
        return;

    list->front = list->back = list_node_init();
    list->front->data = data;
    list->size++;
}

void list_append(list_t *list, void *data) {
    if(list->size == 0) {
        list_add_first(list, data);
        return;
    }

    list->back->next = list_node_init();
    list->back = list->back->next;
    list->back->data = data;
    list->size++;
}

void list_prepend(list_t *list, void *data) {
    if(list->size == 0) {
        list_add_first(list, data);
        return;
    }

    list_node_t* node = list_node_init();
    node->data = data;
    node->next = list->front;
    list->front = node;
    list->size++;
}

void list_insert(list_t *list, void *data, int pos) {
    if(list->size == 0) {
        list_add_first(list, data);
        return;
    }

    if(pos <= 0)
        list_prepend(list, data);
    
    if(pos >= list->size)
        list_append(list, data);

    list_node_t *node = list_node_init();
    list_node_t *prior = list_get_raw(list, pos - 1);
    node->data = data;
    node->next = prior->next;
    prior->next = node;
}

int list_size(list_t *list) {
    return list->size;
}

void* list_get(list_t *list, int pos) {
    if(pos < 0 || pos > list->size - 1)
        return NULL;

    return list_get_raw(list, pos)->data;
}

int list_find(list_t *list, void *data) {
    int i;
    list_node_t *ptr = list->front;
    for(i = 0; i < list->size; ++i) {
        if(ptr->data == data)
            return i;

        ptr = ptr->next;
    }

    return -1;
}

int list_func_find(list_t *list, listfindfptr ffptr) {
    int i;
    list_node_t *ptr = list->front;
    for(i = 0; i < list->size; ++i) {
        if((*ffptr)(ptr->data) == TRUE)
            return i;

        ptr = ptr->next;
    }

    return -1;
}

void* list_remove(list_t *list, int pos) {
    if(pos <= 0)
        list_remove_front(list);

    list_node_t *prior = list_get_raw(list, pos-1);
    list_node_t *node = prior->next;
    prior->next = node->next;
    void *data = node->data;
    free(node);

    if(prior->next == NULL)
        list->back = prior;

    return data;
}

void* list_remove_front(list_t *list) {
    list_node_t *after = list->front->next;
    void *data = list->front->data;
    free(list->front);
    list->front = after;

    return data;
}

void* list_remove_back(list_t *list) {
    return list_remove(list, list->size - 1);
}

void list_remove_item(list_t *list, void *item) {
    int pos;
    if((pos = list_find(list, item)) != -1)
        list_remove(list, pos);
}

void list_iter_reset(list_t *list) {
    list->iterator = NULL;
    list->iter_prev = NULL;
    list->iter_pos = -1;
}

void* list_iter_next(list_t *list) {
    if(list->iter_pos == -1) {
        list->iter_prev = list->iterator;
        list->iterator = list->front;
    } else if(list->iterator != NULL) {
        list->iter_prev = list->iterator;
        list->iterator = list->iterator->next;
    } else
        return NULL;

    list->iter_pos++;
    return list->iterator == NULL ? NULL : list->iterator->data;
}

void* list_iter_get(list_t *list) {
    if(list->iterator == NULL)
        return NULL;

    return list->iterator->data;
}

void* list_iter_remove(list_t *list) {
    if(list->iter_prev == NULL && list->iter_pos != 0)
        return NULL;

    list_node_t *after = list->iterator->next;
    void *data = list->iterator->data;
    free(list->iterator);
    list->iterator = list->iter_prev;
    list->iter_prev = NULL;

    if(list->iterator == NULL)
        list->front = after;
    else
        list->iterator->next = after;

    if(after == NULL)
        list->back = list->iterator;

    list->size--;
    list->iter_pos--;
    return data;
}

void list_iter_insert_before(list_t *list, void *data) {
    if(list->size == 0) {
        list_add_first(list, data);
        return;
    }

    list_node_t *node = list_node_init();
    node->data = data;
    node->next = list->iterator;

    if(list->iter_pos == 0)
        list->front = node;
    else 
        list->iter_prev->next = node;

    list->iter_prev = node;
    list->size++;
    list->iter_pos++;
}

void list_iter_insert_after(list_t *list, void *data) {
    if(list->size == 0) {
        list_add_first(list, data);
        return;
    }

    list_node_t *node = list_node_init();
    node->data = data;
    node->next = list->iterator->next;
    list->iterator->next = node;

    if(node->next == NULL)
        list->back = node;

    list->size++;
}

int list_iter_pos(list_t *list) {
    return list->iter_pos;
}

void list_free(list_t *list) {
    list_node_t *ptr = list->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

void list_nodes_free(list_t *list) {
    list_node_t *ptr = list->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = next;
    }
}

void list_nodes_func_free(list_t *list, listfreefptr ffptr) {
    list_node_t *ptr = list->front, *next;
    while(ptr != NULL) {
        next = ptr->next;
        (*ffptr)(ptr->data);
        free(ptr);
        next = ptr;
    }
}

