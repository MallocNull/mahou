#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <stdint.h>
#include "common.h"

typedef struct user_t user_t;
struct user_t {
    queue_t *in_packets;
    pthread_mutex_t mx_in_packets;
    
    queue_t *out_packets;
    pthread_mutex_t mx_out_packets;
    
    socket_t *sock;
    uint16_t context_id;
    uint16_t x, y;
};

void user_ctx_init();

void user_ctx_free();

#endif