#include "user.h"

const char* _SKILL_NAMES[] = {
    "Bare-Handed",
    "Melee",
    "Ranged",
    "White Magic",
    "Black Magic",
    "Firemaking"
};

struct {
    pthread_mutex_t mx_ctx;
    list_t *active_users;
    user_t **users;
} static ctx;

static BOOL user_check_flag_nomx(user_t *user, uint64_t flag);

void user_context_init() {
    pthread_mutex_init(&ctx.mx_ctx, NULL);
    ctx.active_users = list_init();
    ctx.users = (user_t**)malloc(MAX_CONNS * sizeof(user_t*));
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        ctx.users[i] = NULL;
}

BOOL user_context_add(socket_t *sock) {
    pthread_mutex_lock(&ctx.mx_ctx);
    
    int i;
    for(i = 0; i < MAX_CONNS && ctx.users[i] != NULL; ++i);
    if(i >= MAX_CONNS) {
        pthread_mutex_unlock(&ctx.mx_ctx);
        return FALSE;
    }
    
    user_t *user = user_init(sock, i);
    ctx.users[i] = user;
    list_append(ctx.active_users, user);
    
    pthread_mutex_unlock(&ctx.mx_ctx);
    return TRUE;
}

void user_context_remove(user_t *user) {
    if(user_check_flag_nomx(user, USER_FLAG_DELETING))
        return;
    
    pthread_mutex_lock(&ctx.mx_ctx);
    
    pthread_mutex_lock(&user->mx_user);
    user->flags |= USER_FLAG_DELETING;
    list_remove_item(ctx.active_users, user);
    ctx.users[user->user_id] = NULL;
    user_free(user);
    
    pthread_mutex_unlock(&ctx.mx_ctx);
}

void user_context_free() {
    pthread_mutex_lock(&ctx.mx_ctx);
    list_free(ctx.active_users);
    
    int i;
    for(i = 0; i < MAX_CONNS; ++i)
        user_free(ctx.users[i]);
    
    free(ctx.users);
}

user_t* user_init(socket_t *sock, uint16_t user_id) {
    user_t *user = (user_t*)malloc(sizeof(user_t));
    pthread_mutex_init(&user->mx_user, NULL);
	user->user_id = user_id;
	user->sock = sock;
    user->flags = 0;
	
	user->in_packets = queue_init();
	user->out_packets = queue_init();
    
    return user;
}

BOOL user_check_flag_nomx(user_t *user, uint64_t flag) {
    return (user->flags & flag) != 0;
}
    

BOOL user_check_flag(user_t *user, uint64_t flag) {
    pthread_mutex_lock(&user->mx_user);
	BOOL retval = user_check_flag_nomx(user, flag);
    pthread_mutex_unlock(&user->mx_user);
    return retval;
}

void user_free(user_t *user) {
    if(user == NULL) 
        return;
    
    packet_t *pck = NULL;
    
    while((pck = (packet_t*)queue_pop(user->in_packets)) != NULL)
        packet_free(pck);
    queue_free(user->in_packets);
    
    while((pck = (packet_t*)queue_pop(user->out_packets)) != NULL)
        packet_free(pck);
    queue_free(user->out_packets);
    
    free(user);
}