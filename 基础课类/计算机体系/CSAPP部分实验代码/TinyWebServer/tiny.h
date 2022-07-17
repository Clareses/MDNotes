#include "rio.h"

/**
 * @description: 处理一个http请求 
 * @param fd int 文件描述符
 * @return *
 * @example: 
 */
void doit(int fd);

/**
 * @description: doit函数的辅助函数，用于处理请求的起始行
 * @param *rp rio_t 读取缓冲区
 * @return *
 * @example: 
 */
void read_requesthdrs(rio_t *rp);

/**
 * @description: uri解析函数
 * @param *uri char 统一资源定位符字符串
 * @param *filename char 文件名字符串
 * @param *cgiargs char CGI参数字符串
 * @return *
 * @example: 
 */
int parse_uri(char *uri, char *filename, char *cgiargs);

/**
 * @description: 静态文件处理函数
 * @param fd int 文件描述符
 * @param *filename char 文件名字符串
 * @param filesize int 文件大小
 * @return *
 * @example: 
 */
void serve_static(int fd, char *filename, int filesize);

/**
 * @description: 获取文件类型
 * @param *filename char 文件名字符串
 * @param *filetype char 文件类型字符串
 * @return *
 * @example: 
 */
void get_filetype(char *filename, char *filetype);

/**
 * @description: 动态文件处理函数
 * @param fd int 文件描述符
 * @param *filename char 文件名字符串
 * @param *cgiargs char CGI参数字符串
 * @return *
 * @example: 
 */
void serve_dynamic(int fd, char *filename, char *cgiargs);

/**
 * @description: 客户端错误处理函数
 * @param fd int 文件描述符
 * @param *cause char 错误原因字符串
 * @param *errnum char 错误编号字符串
 * @param *shortmsg char 短消息字符串
 * @param *longmsg char 长消息字符串
 * @return *
 * @example: 
 */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);