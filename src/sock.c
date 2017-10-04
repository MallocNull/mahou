#include "sock.h"

static socket_t* sock_init() {
    return (socket_t*)malloc(sizeof(socket_t));
}

socket_t* sock_client_init(char *addr, char *port) {
    socket_t* sock = sock_init();
    sock->type = SOCK_TYPE_CLIENT;
    sock->port = port;
    sock->addr = addr;
}

socket_t* sock_server_init(char *port) {
    socket_t* sock = sock_init();
    sock->type = SOCK_TYPE_SERVER;
    sock->port = port;
}

static int sock_server_start(socket_t *sock, struct addrinfo *hints) {
    struct addrinfo *result;
    if(getaddrinfo(NULL, sock->port, hints, &result) != 0)
        return SOCK_ERR_GETADDRINFO;

    sock->socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(sock->socket == NULL) {
        freeaddrinfo(result);
        return SOCK_ERR_SOCKET;
    }

    if(bind(sock->socket, result->ai_addr, (int)result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        close(sock->socket);
        return SOCK_ERR_BIND;
    }

    freeaddrinfo(result);
    if(listen(sock->socket, SOMAXCONN) < 0) {
        close(sock->socket);
        return SOCK_ERR_LISTEN;
    }

    return 0;
}

static int sock_client_start(socket_t *sock, struct addrinfo *hints) {
    struct addrinfo *results, *ptr;
    if(getaddrinfo(sock->addr, sock->port, hints, &results) != 0)
        return SOCK_ERR_GETADDRINFO;

    for(ptr = results; ptr != NULL; ptr = ptr->ai_next) {
        if((sock->socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) < 0) {
            freeaddrinfo(results);
            return SOCK_ERR_SOCKET;
        }

        if(connect(sock->socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0)
            break;

        close(sock->socket);
        sock->socket = -1;
    }

    freeaddrinfo(results);
    if(sock->socket < 0)
        return SOCK_ERR_CONNECT;

    return 0;
}

int sock_start(socket_t *sock) {
    struct addrinfo hints;
    bzero((char*)&hints, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

    if(sock->type == SOCK_TYPE_CLIENT)
        return sock_client_start(sock, &hints);
    else
        return sock_server_start(sock, &hints);
}

void sock_set_timeout(socket_t *sock, int secs, int msecs) {
    struct timeval timeout;
    timeout.tv_sec = secs;
    timeout.tv_usec = msecs * 1000;

    setsockopt(sock->socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(sock->socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
}

void sock_set_blocking(socket_t *sock) {
    int flags = fcntl(sock->socket, F_GETFL, 0);
    flags &= ~O_NONBLOCK;
    fcntl(sock->socket, F_SETFL, flags);
}

void sock_set_nonblocking(socket_t *sock) {
    int flags = fcntl(sock->socket, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sock->socket, F_SETFL, flags);
}

BOOL sock_is_blocking(socket_t *sock) {
    return (fcntl(sock->socket, F_GETFL, 0) & O_NONBLOCK) == 0;
}

socket_t* sock_accept(socket_t *sock) {
    int hconn;
    struct sockaddr_in addr = {0};
    unsigned int addrlen = sizeof(addr);

    hconn = accept(sock->socket, (struct sockaddr*)&addr, &addrlen);
    if(hconn < 0 && (errno == EWOULDBLOCK || errno == EAGAIN))
        return NULL;
    else if(hconn < 0)
        return -1;

    socket_t *conn = sock_init();
    conn->type = SOCK_TYPE_SERVING;
    conn->socket = hconn;
    conn->addr_in = addr;
    conn->addrlen = addrlen;

    return conn;
}

int sock_recv(socket_t *sock, char *buffer, int length) {
    if(sock->type == SOCK_TYPE_SERVER)
        return -1;

    int got = read(sock->socket, buffer, length);
    if(got < 0 && (errno == EWOULDBLOCK || errno == EAGAIN))
        return 0;
    else if(got <= 0)
        return -1;

    return got;
}

int sock_send(socket_t *sock, char *buffer, int length) {
    if(sock->type == SOCK_TYPE_SERVER)
        return -1;

    int sent = write(sock->socket, buffer, length);
    if(sent < 0)
        return -1;

    return sent;
}

void sock_stop(socket_t *sock) {
    shutdown(sock->socket, SHUT_RDWR);
    close(sock->socket);
}

void sock_free(socket_t *sock) {
    free(sock);
}

