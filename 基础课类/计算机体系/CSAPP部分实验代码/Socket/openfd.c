#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int open_clientfd(char* hostname, char* port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(hostname, port, &hints, &listp);
    for (p = listp; p != NULL; p = p->ai_next) {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) <
            0) {
            continue;
        }
        if ((connect(clientfd, p->ai_addr, p->ai_addrlen)) != -1)
            break;
        close(clientfd);
    }
    freeaddrinfo(listp);
    if (!p) {
        return -1;
    } else {
        return clientfd;
    }
}

int open_listenfd(char* hostname, char* port) {
    int listenfd;
    int optval = 1;
    struct addrinfo hints, *listp, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(hostname, port, &hints, &listp);
    for (p = listp; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0)
            continue;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(listenfd);
    }
    freeaddrinfo(listp);
    if (!p)
        return -1;
    if (listen(listenfd, 1024) < 0)
        return -1;
    return listenfd;
}