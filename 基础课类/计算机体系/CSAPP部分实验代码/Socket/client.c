#include "openfd.c"
#include "stdio.h"

int main(){
    int clientfd = open_clientfd("127.0.0.1","99");
    char buf[100];
    while(fgets(buf,100,stdin)!=NULL){
        write(clientfd,buf,strlen(buf));
        read(clientfd,buf,100);
        fputs(buf,stdout);
    }
    close(clientfd);
    return 0;
}