#include "packet.h"

typedef struct packet_ctx_t packet_ctx_t;
struct packet_ctx_t {
    uint8_t iter_start;
    uint32_t iter_length;
    
    uint8_t region_count;
    uint16_t *region_lengths;
    uint32_t length;
};

struct {
    uint8_t count;
    packet_ctx_t *c2s;
    packet_ctx_t *s2c;
    
    packet_ctx_t *in;
    packet_ctx_t *out;
} static ctx;

void packet_context_init(uint8_t count) {
    ctx.count = count;
    ctx.c2s = (packet_ctx_t*)malloc(count * sizeof(packet_ctx_t));
    ctx.s2c = (packet_ctx_t*)malloc(count * sizeof(packet_ctx_t));
    
    uint8_t   spawn_type  = get_spawn_type();
    ctx.in  = spawn_type == SPAWN_CLIENT ? ctx.s2c : ctx.c2s;
    ctx.out = spawn_type == SPAWN_CLIENT ? ctx.c2s : ctx.s2c;
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
        uint16_t length = va_arg(args, int);
        ptr[id].region_lengths[i] = length;
        ptr[id].length += length;
    }
    va_end(args);
}

void packet_context_free() {
    int i;
    
    for(i = 0; i < ctx.count; ++i)
        free(ctx.c2s[i].region_lengths);
    free(ctx.c2s);
    
    for(i = 0; i < ctx.count; ++i)
        free(ctx.s2c[i].region_lengths);
    free(ctx.s2c);
}

static void packet_fill_regions(packet_t *packet) {
    uint8_t *ptr = packet->raw;
    int i;
    
    packet->regions = (uint8_t**)malloc(packet->region_count * sizeof(uint8_t*));
    for(i = 0; i < packet->region_count; ++i) {
        packet->regions // TOD Ofisnibh THIS 
}

packet_t* packet_init_in(uint8_t *raw) {
    if(raw[0] != 0xDE || raw[1] != 0xAD)
        return NULL;
    if(raw[2] > ctx.count)
        return NULL;
    
    packet_ctx_t *in = &(ctx.in[raw[2]]);
    
    packet_t *packet = (packet_t*)malloc(sizeof(packet_t));
    packet->id = raw[2];
    packet->raw = raw;
    packet->length = ntohl(*((uint32_t*)raw[3]));
    packet->region_lengths = in->region_lengths;
    packet->iterator = 0;
    packet->next_append = 0xFFFF;
    packet_fill_regions(packet);
    
    return packet;
}

packet_t* packet_init_out(uint8_t id, uint16_t iterations) {
    if(id > ctx.count)
        return NULL;
    
    packet_ctx_t *out = &(ctx.out[id]);
    int regions = out->region_count;
    int iterable_regions = out->region_count - out->iter_start - 1;
    if(out->iter_start != 0)
        regions = out->iter_start + (iterations * iterable_regions);
    
    packet_t *packet = (packet_t*)malloc(sizeof(packet_t));
    
    packet->id = id;
    packet->iterator = 0xFFFF;
    packet->next_append = 0;
    packet->region_count = regions;
    packet->region_lengths = out->region_lengths;
    packet->length = (length - iter_length) + iter_length * iterations;
    packet->raw = (uint8_t*)malloc(packet->length * sizeof(uint8_t));
    packet_fill_regions(packet);
    
    return packet;
}