#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"
#include "queue.h"
#include "sock.h"
#include "common.h"

#define USER_FLAG_LOGGED    1
#define USER_FLAG_DELETING  2

typedef struct user_t user_t;
struct user_t {
    pthread_mutex_t mx_user;
    
    queue_t *in_packets;
    queue_t *out_packets;
    
    socket_t *sock;
    uint64_t flags;
    uint16_t user_id, context_id;
    uint16_t x, y;
};

void user_context_init();
void user_context_add(user_t*);
void user_context_remove(user_t*);
void user_context_free();

user_t* user_init(socket_t*);
BOOL user_check_flag(user_t*, uint64_t);
void user_free(user_t*);

#endif