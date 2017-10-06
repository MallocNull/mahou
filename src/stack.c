#include "stack.h"

stack_t* stack_init() {
    stack_t *stack = (stack_t*)malloc(sizeof(stack_t));
    stack->next = NULL;
    stack->data = NULL;

    return stack;
}

void stack_push(stack_t *stack, void *data) {
    stack_t *node = stack_init();
    node->data = data;
    node->next = stack->next;
    stack->next = node;
}

void* stack_pop(stack_t *stack) {
    if(stack_can_pop(stack) == FALSE)
        return NULL;

    stack_t *this = stack->next;
    void *data = this->data;
    stack->next = this->next;
    free(this);

    return data;
}

BOOL stack_can_pop(stack_t *stack) {
    return stack->next != NULL;
}

void stack_free(stack_t *stack) {
    stack_t *ptr = stack, *next;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

void stack_nodes_free(stack_t *stack) {
    stack_t *ptr = stack, *next;
    while(ptr != NULL) {
        next = ptr->next;
        if(ptr->data != NULL)
            free(ptr->data);

        free(ptr);
        ptr = next;
    }
}

void stack_nodes_func_free(stack_t *stack, stackffptr ffptr) {
    stack_t *ptr = stack, *next;
    while(ptr != NULL) {
        next = ptr->next;
        if(ptr->data != NULL)
            (*ffptr)(ptr->data);

        free(ptr);
        ptr = next;
    }
}
