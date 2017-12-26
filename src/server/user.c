#include "user.h"

struct {
    pthread_mutex_t mx_ctx;
    list_t *active_users;
    list_t *pending_removals;
    user_t **users;
} static ctx;

static user_t* user_init(socket_t*, uint16_t);
static BOOL user_check_flag_nomx(user_t *user, uint64_t flag);
static void user_free(user_t*);

void user_context_init() {
    pthread_mutex_init(&ctx.mx_ctx, NULL);
    ctx.active_users = list_init();
    ctx.pending_removals = list_init();
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
    pthread_mutex_lock(&ctx.mx_ctx);
    
    if(list_find(ctx.pending_removals, user) == -1)
        list_append(ctx.pending_removals, user);
    
    pthread_mutex_unlock(&ctx.mx_ctx);
    
    list_remove_item(ctx.active_users, user);
    ctx.users[user->user_id] = NULL;
    user_free(user);
    
}

void user_context_purge() {
    pthread_mutex_lock(&ctx.mx_ctx);
    
    user_t *ptr;
    list_iter_reset(ctx.pending_removals);
    while((ptr = list_iter_next(ctx.pending_removals)) != NULL) {
        list_remove_item(ctx.active_users, ptr);
        ctx.users[ptr->user_id] = NULL;
        user_free(ptr);
        list_iter_remove(ctx.pending_removals);
    }
    
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

static user_t* user_init(socket_t *sock, uint16_t user_id) {
    user_t *user = (user_t*)malloc(sizeof(user_t));
    pthread_mutex_init(&user->mx_user, NULL);
	user->user_id = user_id;
	user->sock = sock;
    user->logged_in = FALSE;
	
	user->in_packets = queue_init();
	user->out_packets = queue_init();
    
    return user;
}

BOOL user_check_flag_nomx(user_t *user, uint64_t flag) {
    return (user->data.flags & flag) != 0;
}
    
BOOL user_check_flag(user_t *user, uint64_t flag) {
    pthread_mutex_lock(&user->mx_user);
	BOOL retval = user_check_flag_nomx(user, flag);
    pthread_mutex_unlock(&user->mx_user);
    return retval;
}

void user_push_in_packet(user_t *user, packet_t *packet) {
    pthread_mutex_lock(&user->mx_user);
    queue_push(user->in_packets, packet);
    pthread_mutex_unlock(&user->mx_user);
}

void user_push_out_packet(user_t *user, packet_t *packet) {
    pthread_mutex_lock(&user->mx_user);
    queue_push(user->out_packets, packet);
    pthread_mutex_unlock(&user->mx_user);
}

packet_t* user_pop_in_packet(user_t *user) {
    pthread_mutex_lock(&user->mx_user);
    packet_t *packet = queue_pop(user->in_packets);
    pthread_mutex_unlock(&user->mx_user);
    return packet;
}

packet_t* user_pop_out_packet(user_t *user) {
    pthread_mutex_lock(&user->mx_user);
    packet_t *packet = queue_pop(user->out_packets);
    pthread_mutex_unlock(&user->mx_user);
    return packet;
}

static void user_free(user_t *user) {
    if(user == NULL) 
        return;
    
    pthread_mutex_lock(&user->mx_user);
    packet_t *pck = NULL;
    
    while((pck = (packet_t*)queue_pop(user->in_packets)) != NULL)
        packet_free(pck);
    queue_free(user->in_packets);
    
    while((pck = (packet_t*)queue_pop(user->out_packets)) != NULL)
        packet_free(pck);
    queue_free(user->out_packets);
    
    free(user);
}