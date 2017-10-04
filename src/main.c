#include <stdio.h>
#include "common.h"
#include "sock.h"
#include "client.h"
#include "server.h"

int main(int argc, char **argv) {
    if(argc == 2 && strcmp(argv[1], "server") == 0) {
        server();
    } else {
        client();
    }
}
