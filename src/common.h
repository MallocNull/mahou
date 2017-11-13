#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WORK

#define MAHOU_PORT "6770"

#define FALSE 0
#define TRUE 1
#define BOOL char

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define KEY_BS 8
#define KEY_TAB 9
#define KEY_LF 10

#define MAX_CONNS 100

uint64_t htonll(uint64_t);
uint64_t ntohll(uint64_t);

#define SPAWN_UNDEF  0
#define SPAWN_CLIENT 1
#define SPAWN_SERVER 2
void register_spawn_type(uint8_t);
uint8_t get_spawn_type(void);

#endif