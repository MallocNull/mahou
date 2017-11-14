#include "context.h"

struct {
    BOOL running;
    pthread_t thread;
} static ctx;

void* server_context(void *params) {
    flimit_t vsync;
    while(ctx.running == TRUE) {
        frame_limit_tick(&vsync);
        
        frame_limit_wait(&vsync, 60);
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