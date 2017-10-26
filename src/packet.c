#include "packet.h"

typedef struct packet_ctx_t packet_ctx_t;
struct packet_ctx_t {
    uint8_t iter_start;
    uint8_t region_count;
    uint16_t *region_lengths;
    uint32_t length;
};

static packet_ctx_t *c2s_ctx;
static packet_ctx_t *s2c_ctx;

