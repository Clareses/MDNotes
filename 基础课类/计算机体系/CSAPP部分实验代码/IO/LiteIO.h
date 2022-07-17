#include "stddef.h"
#define MAX_BUF_SIZE 8192

typedef struct
{
    int ioFd;                 //file description
    int ioCnt;                //还未读取的字节数
    char *ioBufPtr;           //指向buf中还未读取区域的第一个字节
    char ioBuf[MAX_BUF_SIZE]; //缓冲区
} IoBuf;

/**
 * @description: 输出错误信息
 * @param {char} *message
 * @return {*}
 */
void unixError(char *message);

/**
 * @description: 从fd文件中读入n个字节到userBuf中
 * @param {int} fd 文件描述符
 * @param {void*} userBuf 用户缓冲区
 * @param {size_t} n 读入文件个数
 * @return 读入的字节数，错误返回-1
 */
int ioReadn(int fd,void* userBuf,size_t n);

/**
 * @description: 从userBuf中读n个字节写入fd中
 * @param {int} fd 文件描述符
 * @param {void*} userBuf 用户缓冲区
 * @param {size_t} n 写入长度
 * @return error?-1:写入的字节数
 */
int ioWriten(int fd,void* userBuf,size_t n);

/**
 * @description: 初始化缓冲区
 * @param {IoBuf*} readBuf 读缓冲区
 * @param {int} fd 文件描述符
 * @return {*}
 */
void ioReadInitBuf(IoBuf* readBuf,int fd);

/**
 * @description: 带缓冲读入
 * @param {IoBuf} ioBuf 输入缓冲区
 * @param {void*} userBuf 用户缓冲区
 * @param {size_t} n 读入长度
 * @return error?-1:读入字节数
 */
int ioReadnBuf(IoBuf* ioBuf,void* userBuf,size_t n);

/**
 * @description: 带缓冲读入行
 * @param {IoBuf*} ioBuf 读入缓冲区
 * @param {void*} userBuf 用户缓冲区
 * @param {size_t} maxn 最大读入长度
 * @return error?-1:读入字节数
 */
int ioReadLineBuf(IoBuf* ioBuf,void* userBuf,size_t maxn);

