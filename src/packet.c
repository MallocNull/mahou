#include "packet.h"

typedef struct packet_ctx_t packet_ctx_t;
struct packet_ctx_t {
    uint8_t iter_start;
    uint8_t region_count;
    uint16_t *region_lengths;
    uint32_t length;
};

struct {
    uint8_t count;
    packet_ctx_t *c2s;
    packet_ctx_t *s2c;
} static ctx;

void packet_context_init(uint8_t count) {
    ctx.count = count;
    ctx.c2s = (packet_ctx_t*)malloc(count * sizeof(packet_ctx_t));
    ctx.s2c = (packet_ctx_t*)malloc(count * sizeof(packet_ctx_t));
}

void packet_context_register(uint8_t direction, uint8_t id, uint8_t iter_pt, uint8_t count, ...) {
    if(id > ctx.count)
        return;
    
    int i;
    va_list args;
    packet_ctx_t *ptr = 
        direction == PCK_CTX_C2S ? ctx.c2s : ctx.s2c;
        
    ptr[id].length = 0;
    ptr[id].iter_start = iter_pt;
    ptr[id].region_count = count;
    ptr[id].region_lengths = (uint16_t*)malloc(count * sizeof(uint16_t));
    
    va_start(args, count);
    for(i = 0; i < count; ++i) {
        uint16_t length = va_arg(args, uint16_t);
        ptr[id].region_lengths[i] = length;
        ptr[id].length += length;
    }
    va_end(args);
}

void packet_context_free() {
    free(ctx.c2s.region_lengths);
    free(ctx.c2s);
    
    free(ctx.s2c.region_lengths);
    free(ctx.s2c);
}