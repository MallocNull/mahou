#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include "net/sock.h"
#include "net/packet.h"
#include "types/queue.h"
#include "etc/flimit.h"
#include "context.h"
#include "user.h"

void server(void);

#endif
