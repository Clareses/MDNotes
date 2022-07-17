#include "../IO/LiteIO.h"
#include "./openfd.h"
#include "stdio.h"

#define maxLen 100

void echo(int connfd) {
    size_t n;
    char buf[maxLen];
    IoBuf myBuf;
    ioReadInitBuf(&myBuf, n);
    while ((n = ioReadnBuf(&myBuf, buf, 10)) != 0) {
        printf("server received %d bytes\n", (int)n);
        ioWriten(connfd, buf, n);
    }
}

int main(int argc, char** argv) {
    int listenfd;
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[maxLen];
    char client_port[maxLen];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return 0;
    }

    listenfd = open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        getnameinfo(&clientaddr, clientlen, client_hostname, maxLen,
                    client_port, maxLen, 0);
        printf("connect to %s:%s\n", client_hostname, client_port);
        size_t n;
        char buf[maxLen];
        IoBuf myBuf;
        ioReadInitBuf(&myBuf, n);
        while ((n = read(connfd, buf, 10)) != 0) {
            printf("server received %d bytes\n", (int)n);
            ioWriten(connfd, buf, n);
        }
        close(connfd);
        printf("connect over %s:%s\n", client_hostname, client_port);
    }
    return 0;
}