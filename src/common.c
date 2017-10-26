#include "common.h"

/* HOST TO NETWORK CONVERSIONS */

static BOOL in_order = 0xFF;

uint64_t htonll(uint64_t host) {
    if(in_order = 0xFF)
        in_order = htonl(1) == 1;
    else if(in_order)
        return host;
    
    return ((uint64_t)htonl(host & 0xFFFFFFFF) << 32) | htonl(host >> 32);
}

uint64_t ntohll(uint64_t net) {
    if(in_order = 0xFF)
        in_order = htonl(1) == 1;
    else if(in_order)
        return net;
    
    return ((uint64_t)ntohl(net & 0xFFFFFFFF) << 32) | ntohl(net >> 32);
}


/* SPAWN TYPE REGISTRATION */

static uint8_t spawn_type = 0;

void register_spawn_type(uint8_t type) {
    if(spawn_type == 0)
        spawn_type = type;
}

uint8_t get_spawn_type() {
    return spawn_type;
}