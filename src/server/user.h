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

#define USER_FLAG_GENDER    1
  #define USER_GENDER_MALE    0
  #define USER_GENDER_FEMALE  1
#define USER_FLAG_POISONED  2

#define SKILLS_COUNT   5
#define SKILL_MELEE     0
#define SKILL_RANGED    1
#define SKILL_WHMAGIC   2
#define SKILL_BLMAGIC   3
#define SKILL_WOODCUT   4
#define SKILL_FIREMAKE  5
#define SKILL_MINING    6
#define SKILL_SMITHING  7
#define SKILL_FISHING   8
#define SKILL_COOKING   9

#define STATS_COUNT    9
#define STAT_STRENGTH   0
#define STAT_HEALTH     1
#define STAT_DEFENSE    2
#define STAT_AGILITY    3
#define STAT_INTELLECT  4
#define STAT_WISDOM     5
#define STAT_CHARISMA   6
#define STAT_
#define STAT_LUCK       8

extern const char* _SKILL_NAMES[];
extern const char* _SKILL_NAMES_SHORT[];
extern const char* _STAT_NAMES[];
extern const char* _STAT_NAMES_SHORT[];

typedef struct user_t user_t;
struct user_t {
    pthread_mutex_t mx_user;
    
    queue_t *in_packets;
    queue_t *out_packets;
    
    socket_t *sock;
    uint16_t user_id;
    BOOL logged_in;

    struct {
        char name[9];
        uint16_t pin, x, y;
        uint16_t context_id;
        uint64_t flags;
    } data;
    
    uint32_t skills_exp[SKILLS_COUNT];
    uint8_t  base_stats[STATS_COUNT];
};

void user_context_init(void);
BOOL user_context_add(socket_t*);
user_t* user_context_get(uint16_t id);
void user_context_remove(user_t*);
void user_context_purge(void);
void user_context_free(void);

BOOL user_check_flag(user_t*, uint64_t);
void user_push_in_packet(user_t*);
void user_push_out_packet(user_t*);
packet_t* user_pop_in_packet(user_t*);
packet_t* user_pop_out_packet(user_t*);

#endif