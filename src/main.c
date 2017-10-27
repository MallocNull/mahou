#include "common.h"
#include "packet.h"
#include "client/client.h"
#include "server/server.h"

int main(int argc, char **argv) {
    packet_context_init(2);
    
    packet_context_register(PCK_CTX_C2S, PCK_ID_REGISTER,   0, 2, 8, 2);
    packet_context_register(PCK_CTX_C2S, PCK_ID_LOGIN,      0, 2, 8, 2);
    packet_context_register(PCK_CTX_C2S, PCK_ID_CHANGE_CTX, 0, 2, 1, 2);
    
    packet_context_register(PCK_CTX_S2C, PCK_ID_REGISTER,   0, 2, 1, 16);
    packet_context_register(PCK_CTX_S2C, PCK_ID_LOGIN,      0, 3, 1, 2, 16);
    packet_context_register(PCK_CTX_S2C, PCK_ID_CHANGE_CTX, 0, 1, 2);
    
    if(argc == 2 && strcmp(argv[1], "server") == 0) {
        register_spawn_type(SPAWN_SERVER);
        server();
    } else {
        register_spawn_type(SPAWN_CLIENT);
        client();
    }
}
