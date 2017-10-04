#include "client/client.h"
#include "server/server.h"

int main(int argc, char **argv) {
    if(argc == 2 && strcmp(argv[1], "server") == 0) {
        server();
    } else {
        client();
    }
}
