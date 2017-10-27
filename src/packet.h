#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "list.h"
#include "common.h"

#define PCK_CTX_C2S 0
#define PCK_CTX_S2C 1

#define PCK_ID_REGISTER     0
#define PCK_ID_LOGIN        1
#define PCK_ID_CHANGE_CTX   2

typedef struct packet_t packet_t;
struct packet_t {
    uint8_t id;
    uint32_t length;
    uint8_t **regions;
    const uint16_t *region_lengths;
    uint8_t region_count;
    uint8_t iterator;
};

void packet_context_init(uint8_t);

void packet_context_register(uint8_t, uint8_t, uint8_t, ...);
void packet_context_register_iter(uint8_t, uint8_t, uint8_t, uint8_t, ...);

void packet_context_free();

#endif