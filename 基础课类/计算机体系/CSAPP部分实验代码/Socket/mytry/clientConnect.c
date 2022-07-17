#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "stdio.h"

typedef struct myIpSocketAddr{
    u_int16_t sin_family;
    u_int16_t sin_port;
    u_int32_t sin_addr;
    unsigned char sin_zero[8];
}mySA;

typedef struct sockaddr SA;

int main(){
    mySA server;
    server.sin_family = AF_INET;
    server.sin_port = 99;
    printf("Hello\n");
    inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);
    char* buff = (char*)malloc(20*sizeof(char));
    buff[19]=0;
    printf("%s\n",inet_ntop(AF_INET,&server.sin_addr,buff,19));
    int clientFile = socket(AF_INET,SOCK_STREAM,0);
    printf("%d\n",clientFile);
    int tag = connect(clientFile,(SA*)&server,sizeof(struct myIpSocketAddr));
    printf("%d\n",tag);
    char c;
    sleep(5);
    printf("sleep Over\n");
    while(read(clientFile,&c,1) != 0){
        printf("--%c",c);
    }
    close(clientFile);
    return 0;
}