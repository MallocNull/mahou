#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <locale.h>
#include <ncurses.h>
#include "common.h"
#include "net/sock.h"
#include "net/packet.h"
#include "screen.h"
#include "ui.h"

void client(void);

#endif
