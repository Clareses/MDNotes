#include "openfd.c"
#include "stdio.h"

int main(){
    struct sockaddr clientSock;
    char hostname[100];
    char buf[100];
    int connfd;
    socklen_t clientlen;
    int listenfd = open_listenfd("127.0.0.1","99");
    while(1){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd,&clientSock,clientlen);
        getnameinfo(&clientSock,clientlen,buf,50,buf+50,50,0);
        printf("receive a connect from %s:%s\n",buf,buf+50);
        while(read(connfd,buf,100)!=0){
            // printf("I get some thing!\n");
            write(connfd,buf,100);
        }
        close(connfd);
    }
    return 0;
}