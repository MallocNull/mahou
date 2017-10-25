#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>
#include <stdint.h>
#include "list.h"

#define PCK_ID_REGISTER     0
#define PCK_ID_LOGIN        1
#define PCK_ID_CHANGE_CTX   2

typedef struct packet_t packet_t;
struct packet_t {
    uint8_t id;
    uint32_t length;
    uint8_t **regions;
    const uint8_t *region_lengths;
    uint8_t region_count;
};



#endif