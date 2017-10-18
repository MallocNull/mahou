#ifndef FLIMIT_H
#define FLIMIT_H

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef time_t flimit_t;

void frame_limit_tick(flimit_t*);
void frame_limit_wait(flimit_t*, unsigned int);

#endif