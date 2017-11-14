#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "types/list.h"
#include "types/queue.h"
#include "net/sock.h"
#include "net/packet.h"
#include "common.h"

#define USER_FLAG_LOGGED    1
#define USER_FLAG_DELETING  2

#define SKILL_COUNT     6

#define SKILL_BAREHANDS 0
#define SKILL_MELEE     1
#define SKILL_RANGED    2
#define SKILL_WHMAGIC   3
#define SKILL_BLMAGIC   4
#define SKILL_FIREMAKE  5

extern const char* _SKILL_NAMES[];

typedef struct user_t user_t;
struct user_t {
    pthread_mutex_t mx_user;
    
    queue_t *in_packets;
    queue_t *out_packets;
    
    socket_t *sock;
    uint64_t flags;
    uint16_t user_id;

    struct {
        char name[9];
        uint64_t flags;
        uint16_t pin, x, y;
        uint16_t context_id;
        uint32_t skill_exp[SKILL_COUNT];
    } data;
};

void user_context_init(void);
BOOL user_context_add(socket_t*);
void user_context_remove(user_t*);
void user_context_free(void);

user_t* user_init(socket_t*, uint16_t);
BOOL user_check_flag(user_t*, uint64_t);
void user_free(user_t*);

#endif