#include "server.h"

void server() {
    BOOL running = TRUE;
	socket_t *conn;
	
    user_context_init();
    server_context_start();
    
    socket_t *sock = sock_server_init("6770");
    sock_start(sock);
    sock_set_nonblocking(sock);
    
    flimit_t vsync;
    while(running) {
        frame_limit_tick(&vsync);
        
        
        
        for(;;) {
            conn = sock_accept(sock);
            if(conn == NULL)
                break;
            else if(conn == -1) {
                running = FALSE;
                break;
            } else {
                
            }
        }
        
        frame_limit_wait(&vsync, 1000);
    }

    server_context_stop();
    user_context_free();
    
    sock_stop(sock);
    sock_free(sock);
    
    /*printf("awaiting connection...\r\n");
    socket_t *conn = sock_accept(sock);

    printf("connected\r\n");
    sock_send(conn, "test", 5);
    printf("sent message\r\n");

    char in[10];
    sock_recv(conn, in, 10);
    printf("got %s\r\n", in);*/
}
