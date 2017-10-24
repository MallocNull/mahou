#include "server.h"

void server() {
    server_context_start();
    
    socket_t *sock = sock_server_init("6770");
    sock_start(sock);
    sock_set_nonblocking(sock);
    
    
    
    flimit_t vsync;
    for(;;) {
        frame_limit_tick(&vsync);
        
        
        
        frame_limit_wait(&vsync, 1000);
    }
    
    printf("awaiting connection...\r\n");
    socket_t *conn = sock_accept(sock);

    printf("connected\r\n");
    sock_send(conn, "test", 5);
    printf("sent message\r\n");

    char in[10];
    sock_recv(conn, in, 10);
    printf("got %s\r\n", in);

    server_context_stop();
    sock_stop(sock);
    sock_free(sock);
}
