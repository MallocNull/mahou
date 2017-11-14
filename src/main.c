#include "common.h"
#include "client/client.h"
#include "server/server.h"
#include "net/packet.h"

int main(int argc, char **argv) {
	uint8_t spawn_type = 
		argc == 2 && strcmp(argv[1], "server") == 0 
			? SPAWN_SERVER
			: SPAWN_CLIENT;
	
	register_spawn_type(spawn_type);
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
    packet_context_register(PCK_CTX_S2C, PCK_ID_PROMPT_LIST, 5, 6, 1, 1, 2, 512, 2, 256);
    packet_context_register(PCK_CTX_S2C, PCK_ID_CHANGE_CTX,  0, 1, 2);
    
    if(spawn_type == SPAWN_SERVER)
        server();
    else
        client();
	
	return 0;
}
