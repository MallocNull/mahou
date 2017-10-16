#include "server.h"

void server() {
    socket_t *sock = sock_server_init("6770");
    sock_start(sock);

    sock_set_timeout_us(sock, 0, 100);
    
    
    printf("awaiting connection...\r\n");
    socket_t *conn = sock_accept(sock);

    printf("connected\r\n");
    sock_send(conn, "test", 5);
    printf("sent message\r\n");

    char in[10];
    sock_recv(conn, in, 10);
    printf("got %s\r\n", in);

    sock_stop(sock);
    sock_free(sock);
}
