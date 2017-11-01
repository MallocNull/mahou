#include "packet.h"

typedef struct packet_ctx_t packet_ctx_t;
struct packet_ctx_t {
    uint8_t iter_start;
    uint32_t iter_length;
    uint32_t pre_iter_length;
    
    uint8_t region_count;
    uint16_t iter_count;
    uint16_t pre_iter_count;
    
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
    ptr[id].iter_length = 0;
    ptr[id].pre_iter_length = 0;
    ptr[id].iter_start = iter_pt;
    ptr[id].iter_count = count - iter_pt;
    ptr[id].pre_iter_count = count - ptr[id].iter_count;
    ptr[id].region_count = count;
    ptr[id].region_lengths = (uint16_t*)malloc(count * sizeof(uint16_t));
    
    va_start(args, count);
    for(i = 0; i < count; ++i) {
        uint16_t length = va_arg(args, int);
        ptr[id].region_lengths[i] = length;
        ptr[id].length += length;
        if(iter_pt == 0 || i < iter_pt)
            ptr[id].pre_iter_length += length;
        else
            ptr[id].iter_length += length;
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

static void packet_fill_regions(packet_t *packet, packet_ctx_t *ctx) {
    uint8_t *ptr = packet->raw + 7;
    int i;
    
    packet->regions = (uint8_t**)malloc(packet->region_count * sizeof(uint8_t*));
    for(i = 0; i < packet->region_count; ++i) {
        packet->regions[i] = ptr;
        if(ctx->iter_start == 0 || i < ctx->iter_start)
            ptr += packet->region_lengths[i];
        else if(ctx->iter_start != 0)
            ptr += packet->region_lengths[ctx->iter_start + ((i - ctx->iter_start) % ctx->iter_count)];
    }
}

packet_t* packet_init_in(uint8_t *raw) {
    if(raw[0] != 0xDE || raw[1] != 0xAD)
        return NULL;
    if(raw[2] > ctx.count)
        return NULL;
    
    packet_ctx_t *in = &(ctx.in[raw[2]]);
    
    packet_t *packet       = (packet_t*)malloc(sizeof(packet_t));
    packet->ctx            = in;
    packet->id             = raw[2];
    packet->iterator       = 0;
    packet->next_append    = 0xFFFF;
    packet->raw            = raw;
    packet->length         = ntohl(*((uint32_t*)&raw[3]));
    packet->region_lengths = in->region_lengths;
    packet->region_count   = in->pre_iter_count + (packet->length - in->pre_iter_length) / in->iter_length;
    packet_fill_regions(packet, in);
    
    return packet;
}

packet_t* packet_init_out(uint8_t id, uint16_t iterations) {
    if(id > ctx.count)
        return NULL;
    
    packet_ctx_t *out = &(ctx.out[id]);
    int regions = out->region_count;
    if(out->iter_start != 0)
        regions = out->iter_start + (iterations * out->iter_count);
    
    packet_t *packet       = (packet_t*)malloc(sizeof(packet_t));
    packet->ctx            = out;
    packet->id             = id;
    packet->iterator       = 0xFFFF;
    packet->next_append    = 0;
    packet->region_count   = regions;
    packet->region_lengths = out->region_lengths;
    packet->length         = out->pre_iter_length + out->iter_length * iterations;
    packet->raw            = (uint8_t*)malloc(packet->length * sizeof(uint8_t));
    packet_fill_regions(packet, out);
    
    return packet;
}

uint8_t* packet_get(packet_t *packet, uint16_t region, int *size) {
    if(region >= packet->region_count)
        return NULL;
    
    packet_ctx_t *ctx;
    if(size != NULL) {
        ctx = (packet_ctx_t*)packet->ctx;
        if(region >= ctx->iter_start && ctx->iter_start != 0)
            *size = packet->region_lengths[(region - ctx->iter_start) % ctx->iter_count];
        else
            *size = packet->region_lengths[region];
    }
    
    return packet->regions[region];
}

uint8_t packet_get_uint8(packet_t *packet, uint16_t region) {
    if(region >= packet->region_count)
        return NULL;
    
    return *((uint8_t*)packet->regions[region]);
}

uint16_t packet_get_uint16(packet_t *packet, uint16_t region) {
    if(region >= packet->region_count)
        return NULL;
    
    return *((uint16_t*)packet->regions[region]);
}

uint32_t packet_get_uint32(packet_t *packet, uint16_t region) {
    if(region >= packet->region_count)
        return NULL;
    
    return *((uint32_t*)packet->regions[region]);
}

uint64_t packet_get_uint64(packet_t *packet, uint64_t region) {
    if(region >= packet->region_count)
        return NULL;
    
    return *((uint64_t*)packet->regions[region]);
}

uint8_t* packet_get_raw(packet_t *packet, int *size) {
    if(size != NULL)
        *size = packet->length;
    
    return packet->raw;
}

uint16_t packet_region_length(uint64_t todo_finish_this, void *DO_IT