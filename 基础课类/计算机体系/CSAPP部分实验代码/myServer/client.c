#include "./openfd.h"
#include "../IO/LiteIO.h"
#include "stdio.h"

#define maxLen 100

int main(int argc,char** argv){
    int clientfd;
    char* host;
    char* port;
    IoBuf myBuf;
    char buf[maxLen];

    if(argc != 3){
        fprintf(stderr,"usage: %s <hostname> <port>",argv[0]);
        return 0;
    }
    
    host = argv[1];
    port = argv[2];

    clientfd = open_clientfd(host,port);

    ioReadInitBuf(&myBuf,clientfd);

    while(fgets(buf,maxLen,stdin) != NULL){
        ioWriten(clientfd,buf,strlen(buf));
        read(clientfd,buf,10);
        fputs(buf,stdout);
    }
    close(clientfd);
    return 0;
}