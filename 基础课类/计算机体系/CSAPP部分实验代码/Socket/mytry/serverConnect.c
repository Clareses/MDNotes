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
    SA temp;
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    mySA server;
    server.sin_family = AF_INET;
    server.sin_port = 99;
    inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);
    bind(serverSocket,(SA*)&server,sizeof(mySA));
    listen(serverSocket,20);
    while(1){
        int file = accept(serverSocket,&temp,sizeof(SA));
        for(int i = 0;i<14;i++){
            printf("%d",temp.sa_data[i]);
        }
        printf("\n");
        close(file);
    }
    
}