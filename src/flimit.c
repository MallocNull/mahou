#include "flimit.h"

void frame_limit_tick(flimit_t *fld) {
    time(fld);
}

void frame_limit_wait(flimit_t *fld, unsigned int fps) {
    double fr = 1.0 / fps, dt, wait;
    struct timespec swait;
    
    time_t now = time(NULL);
    dt = difftime(now, *fld);
    
    if(dt < fr) {
        wait = fr - dt;
        usleep((unsigned int)(wait * 1000000));
    }
}