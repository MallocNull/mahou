#include "common.h"
#include "packet.h"
#include "client/client.h"
#include "server/server.h"

int main(int argc, char **argv) {
    packet_context_init(5);
    
    packet_context_register(PCK_CTX_C2S, PCK_ID_CLERIC_REQ,  0, 1, 1);
    packet_context_register(PCK_CTX_C2S, PCK_ID_ALERT,       0, 2, 1, 1);
    packet_context_register(PCK_CTX_C2S, PCK_ID_PROMPT_BOOL, 0, 3, 1, 1, 1);
    packet_context_register(PCK_CTX_C2S, PCK_ID_PROMPT_STR,  0, 3, 1, 1, 256);
    packet_context_register(PCK_CTX_C2S, PCK_ID_PROMPT_LIST, 0, 3, 1, 1, 2);
    packet_context_register(PCK_CTX_C2S, PCK_ID_CHANGE_CTX,  0, 2, 1, 2);
    
    packet_context_register(PCK_CTX_S2C, PCK_ID_CLERIC_REQ,  0, 2, 1, 1);
    packet_context_register(PCK_CTX_S2C, PCK_ID_ALERT,       0, 4, 1, 1, 2, 512);
    packet_context_register(PCK_CTX_S2C, PCK_ID_PROMPT_BOOL, 0, 4, 1, 1, 2, 512);
    packet_context_register(PCK_CTX_S2C, PCK_ID_PROMPT_STR,  0, 5, 1, 1, 2, 512, 1);
    packet_context_register(PCK_CTX_S2C, PCK_ID_PROMPT_LIST, 0, 6, 1, 1, 2, 512, 1, 256);
    packet_context_register(PCK_CTX_S2C, PCK_ID_CHANGE_CTX,  0, 1, 2);
    
    if(argc == 2 && strcmp(argv[1], "server") == 0) {
        register_spawn_type(SPAWN_SERVER);
        server();
    } else {
        register_spawn_type(SPAWN_CLIENT);
        client();
    }
}
