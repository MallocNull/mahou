#include "user.h"

struct {
    list_t *active_users;
    user_t **users;
} ctx;

void user_context_init() {
    ctx.active_users = list_init();
    ctx.users = (user_t**)malloc(MAX_CONNS * sizeof(user_t*));
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        ctx.users[i] = NULL;
}

void user_context_free() {
    list_free(ctx.active_users);
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        free(ctx.users[i]);
    
    free(ctx.users);
}