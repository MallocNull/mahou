#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WORK

#define MAHOU_PORT "6770"
#define MAX_CONNS 100

#define FALSE 0
#define TRUE 1
#define BOOL char

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define KEY_CC   3  
#define KEY_BS   8
#define KEY_TAB  9
#define KEY_LF  10

/** SKILL SECTION START **/
#define SKILLS_COUNT   10
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
#define STAT_PERCEPT    7
#define STAT_LUCK       8

extern const char* _SKILL_NAMES[];
extern const char* _SKILL_NAMES_SHORT[];
extern const char* _STAT_NAMES[];
extern const char* _STAT_NAMES_SHORT[];
/** SKILL SECTION END **/

uint64_t htonll(uint64_t);
uint64_t ntohll(uint64_t);

#define SPAWN_UNDEF  0
#define SPAWN_CLIENT 1
#define SPAWN_SERVER 2
void register_spawn_type(uint8_t);
uint8_t get_spawn_type(void);

#endif