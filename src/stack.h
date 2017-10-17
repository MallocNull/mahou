#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "common.h"

typedef void(*stackffptr)(void*);

typedef struct stack_t stack_t;
struct stack_t {
    stack_t *next;
    void *data;
};

stack_t* stack_init();

void stack_push(stack_t*, void*);
void* stack_pop(stack_t*);
BOOL stack_can_pop(stack_t*);
int stack_depth(stack_t*);

void stack_free(stack_t*);
void stack_nodes_free(stack_t*);
void stack_nodes_func_free(stack_t*, stackffptr); 

#endif
