#include "LiteIO.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"

int main(){
    int f = open("./file.txt",0);
    IoBuf myBuf;
    char buf[100];
    ioReadInitBuf(&myBuf,f);
    ioReadLineBuf(&myBuf,buf,100);
    printf("%s",buf);
    return 0;
}