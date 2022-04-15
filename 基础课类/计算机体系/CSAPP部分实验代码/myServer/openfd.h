#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @description: 初始化client fd
 * @param {char*} hostname
 * @param {char*} port
 * @return error?-1:clientfd
 */
int open_clientfd(char* hostname, char* port);

/**
 * @description: 初始化server
 * @param {char*} port
 * @return error?-1:listenfd
 */
int open_listenfd(char* port);