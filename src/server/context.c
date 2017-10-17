#include "context.h"

struct {
    BOOL running;
    pthread_t thread;
} ctx;

void* server_context(void *params) {
    while(ctx.running == TRUE) {
        
    }
    
    return NULL;
}

void server_context_start() {
    if(ctx.running == TRUE)
        return;
    
    pthread_create(&ctx.thread, NULL, &server_context, NULL);
    ctx.running = TRUE;
}

void server_context_stop() {
    ctx.running = FALSE;
    pthread_join(ctx.thread, NULL);
}