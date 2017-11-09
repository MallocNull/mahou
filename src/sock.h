#ifndef SOCK_H
#define SOCK_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include "common.h"

#define SOCK_SUCCESS 0
#define SOCK_ERR_GETADDRINFO -1
#define SOCK_ERR_SOCKET -2
#define SOCK_ERR_BIND -3
#define SOCK_ERR_LISTEN -4
#define SOCK_ERR_CONNECT -5

// socket is client
#define SOCK_TYPE_CLIENT 0
// socket is server
#define SOCK_TYPE_SERVER 1
// socket is client connection on server's side
#define SOCK_TYPE_SERVING 2

#define SOCK_DIR_SEND 0
#define SOCK_DIR_RECV 1

typedef struct socket_t socket_t;
struct socket_t {
    int type;
    int socket;
    struct sockaddr_in addr_in;
    int addrlen;
    char *port;
    char *addr;
};

socket_t* sock_client_init(char*, char*);
socket_t* sock_server_init(char*);

int sock_start(socket_t*);

// second int is millisecs
void sock_set_timeout(socket_t*, uint8_t, int, int);
// second int is microsecs
void sock_set_timeout_us(socket_t*, uint8_t, int, int);
void sock_set_blocking(socket_t*);
void sock_set_nonblocking(socket_t*);
BOOL sock_is_blocking(socket_t*);

socket_t* sock_accept(socket_t*);
int sock_recv(socket_t*, char*, int);
int sock_send(socket_t*, char*, int);

void sock_stop(socket_t*);
void sock_free(socket_t*);

#endif

