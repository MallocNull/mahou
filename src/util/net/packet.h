#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "types/list.h"
#include "common.h"

#define PCK_CTX_C2S 0
#define PCK_CTX_S2C 1

#define PCK_IN 0
#define PCK_OUT 1

#define PCK_CLERIC_REG   0
#define PCK_CLERIC_LOGIN 1

#define PCK_ID_CLERIC_REQ   0
#define PCK_ID_ALERT        1
#define PCK_ID_PROMPT_BOOL  2
#define PCK_ID_PROMPT_STR   3
#define PCK_ID_PROMPT_LIST  4
#define PCK_ID_CHANGE_CTX   5

typedef struct packet_t packet_t;
struct packet_t {
    uint8_t id;
    uint8_t *raw;
    uint8_t **regions;
    const uint16_t *region_lengths;
    uint32_t length;
    uint16_t region_count;
    uint16_t iterator;
    uint16_t next_append;
    void *ctx;
};

void packet_context_init(uint8_t);
void packet_context_register(uint8_t, uint8_t, uint8_t, uint16_t, ...);
void packet_context_free(void);

packet_t* packet_init_in(uint8_t*);
packet_t* packet_init_out(uint8_t, uint16_t);

uint8_t* packet_get        (packet_t*, uint16_t, int*);
uint8_t* packet_get_string (packet_t*, uint16_t, int*);
uint8_t  packet_get_uint8  (packet_t*, uint16_t);
uint16_t packet_get_uint16 (packet_t*, uint16_t);
uint32_t packet_get_uint32 (packet_t*, uint16_t);
uint64_t packet_get_uint64 (packet_t*, uint16_t);
uint8_t* packet_get_raw    (packet_t*, int*);

void     packet_iter_reset      (packet_t*);
BOOL     packet_iter_next       (packet_t*);
BOOL     packet_iter_prev       (packet_t*);
uint8_t* packet_iter_get        (packet_t*, int*);
uint8_t* packet_iter_get_string (packet_t*, int*);
uint8_t  packet_iter_get_uint8  (packet_t*);
uint16_t packet_iter_get_uint16 (packet_t*);
uint32_t packet_iter_get_uint32 (packet_t*);
uint64_t packet_iter_get_uint64 (packet_t*);

void packet_append        (packet_t*, uint8_t*, int);
void packet_append_string (packet_t*, uint8_t*, int);
void packet_append_uint8  (packet_t*, uint8_t);
void packet_append_uint16 (packet_t*, uint16_t);
void packet_append_uint32 (packet_t*, uint32_t);
void packet_append_uint64 (packet_t*, uint64_t);

uint16_t packet_region_length   (uint8_t, uint8_t, uint16_t);
uint32_t packet_pre_iter_length (uint8_t, uint8_t);
uint32_t packet_iter_length     (uint8_t, uint8_t);
BOOL     packet_is_length_valid (uint8_t, uint8_t, uint32_t);

void packet_free(packet_t*);

#endif