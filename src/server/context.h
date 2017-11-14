#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H

#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "etc/flimit.h"

void server_context_start(void);
void server_context_stop(void);

#endif