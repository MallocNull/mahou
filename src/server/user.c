#include "user.h"

struct {
    pthread_mutex_t mx_ctx;
    list_t *active_users;
    user_t **users;
} static ctx;

void user_context_init() {
    ctx.mx_ctx = PTHREAD_MUTEX_INITIALIZER;
    ctx.active_users = list_init();
    ctx.users = (user_t**)malloc(MAX_CONNS * sizeof(user_t*));
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        ctx.users[i] = NULL;
}

BOOL user_context_add(socket_t *sock) {
    int i;
    for(i = 0; i < MAX_CONNS && ctx.users[i] != NULL; ++i);
    if(i >= MAX_CONNS)
        return FALSE;
    
    
}

void user_context_remove(user_t *user) {
    if(user_check_flag(user, USER_FLAG_DELETING))
        return;
    
    pthread_mutex_lock(&user->mx_user);
    user->flags |= USER_FLAG_DELETING;
    list_remove_item(ctx.active_users, user);
    users[user->user_id] = NULL;
    
}

void user_context_free() {
    list_free(ctx.active_users);
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        free(ctx.users[i]);
    
    free(ctx.users);
}

user_t* user_init(socket_t *sock) {
    user_t *user = (user_t*)malloc(sizeof(user_t));
    user->mx_user = PTHREAD_MUTEX_INITIALIZER;
    
    
    return user;
}

void user_free(user_t *user) {
    packet_t *pck = NULL;
    
    while((pck = (packet_t*)queue_pop(user->in_packets) != NULL)
        packet_free(pck);
    queue_free(user->in_packets);
    
    while((pck = (packet_t*)queue_pop(user->out_packets) != NULL)
        packet_free(pck);
    queue_free(user->out_packets);
    
    free(user);
}