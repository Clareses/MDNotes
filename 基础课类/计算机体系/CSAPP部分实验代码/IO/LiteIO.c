#include "LiteIO.h"
#include "errno.h"
#include "stdio.h"
#include "unistd.h"

void unixError(char* message) {
    fprintf(stderr, "%s : %s", message, strerror(errno));
}

int ioReadn(int fd, void* userBuf, size_t n) {
    size_t nLeft = n;      //剩余的未读字节数
    int readRes;       //记录每次的read返回值
    char* bufp = userBuf;  //记录缓冲区指针
    //当没读的值仍大于0时
    while (nLeft > 0) {
        readRes = read(fd, bufp, nLeft);
        if (readRes < 0) {
            if (errno == EINTR)
                readRes = 0;
            else
                return -1;
        } else if (readRes == 0) {
            break;
        }
        nLeft -= readRes;
        bufp += readRes;
    }
    //返回读入的个数
    return n - nLeft;
}

int ioWriten(int fd, void* userBuf, size_t n) {
    size_t nLeft = n;
    char* bufp = userBuf;
    int writeRes;
    while (n > 0) {
        writeRes = write(fd, userBuf, nLeft);
        if (writeRes < 0) {
            if (errno == EINTR) {
                writeRes = 0;
            } else {
                return -1;
            }
        } else if (writeRes == 0) {
            break;
        }
        nLeft -= writeRes;
        bufp += writeRes;
    }
    return n - nLeft;
}

void ioReadInitBuf(IoBuf* readBuf, int fd){
    readBuf->ioFd = fd;
    readBuf->ioCnt = 0;
    readBuf->ioBufPtr = readBuf->ioBuf;
}

int ioReadnBuf(IoBuf* ioBuf, void* userBuf, size_t n){
    int nleft = n;
    while(nleft != 1){
        if(ioBuf->ioCnt != 0){
            *((char*)userBuf++) = *(ioBuf->ioBufPtr++);
            ioBuf->ioCnt --;
            nleft --; 
        }else{
            ioBuf->ioCnt = ioReadn(ioBuf->ioFd,ioBuf->ioBuf,MAX_BUF_SIZE);
            ioBuf->ioBufPtr = ioBuf->ioBuf;
            if(ioBuf->ioCnt == 0) break;
            if(ioBuf->ioCnt == -1) return -1;
        }
    }
    *((char*)userBuf) = 0;
    return n-nleft;
}

int ioReadLineBuf(IoBuf* ioBuf, void* userBuf, size_t maxn){
    int nleft = maxn;
    while(nleft != 1){
        if(ioBuf->ioCnt != 0){
            if(*(ioBuf->ioBufPtr) == '\n') break;
            *((char*)userBuf++) = *(ioBuf->ioBufPtr++);
            ioBuf->ioCnt --;
            nleft --; 
        }else{
            ioBuf->ioCnt = ioReadn(ioBuf->ioFd,ioBuf->ioBuf,MAX_BUF_SIZE);
            ioBuf->ioBufPtr = ioBuf->ioBuf;
            if(ioBuf->ioCnt == 0) break;
            if(ioBuf->ioCnt == -1) return -1;
        }
    }
    *((char*)userBuf) = 0;
    return maxn-nleft;
}
