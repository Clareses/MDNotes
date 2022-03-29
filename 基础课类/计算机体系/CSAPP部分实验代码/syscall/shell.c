#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"
#include "stdio.h"

#define CMD_MAX_LENGTH 120
#define MAX_ARGV_NUM 10

//解析命令行参数,并将参数放入argv中
int parseLine(char* buf,char **argv){
    char *delim;//指向定界符的指针
    int argc;//参数数量
    int bg;//解析是否是后台任务，作为放回值放回

    buf[strlen(buf)] = ' ';//用' '替代结尾的'\n'
    
    //直接指向第一个参数的第一位~！
    while(*buf && *buf == ' '){
        buf++;
    }

    argc = 0;

    //每次将delim定位到buf后第一个空格
    while(delim = strchr(buf,' ')){
        //复制buf到argv中
        argv[argc++] = buf;
        //截断字符串
        *delim = '\0';
        buf = delim + 1;
        //无视两个参数之间的任意个空格
        while(*buf && (*buf == ' ')) buf++;
    }

    //根据数据结构完成argvlist的构建
    argv[argc] = NULL;

    //忽略空行
    if(argc == 0) return 1;

    //判定是否后台运行
    if((bg = (*argv[argc-1] == '&'))!= 0){
        //截去&那一位参数
        argv[--argc] = NULL;
    }
    return bg;
}

//判断是否是内建指令~如果是的话就直接执行，不是的话返回
int builtin_command(char **argv){
    //内建指令 quit
    if(!strcmp(argv[0],"quit")) exit(0);
    //忽略&开头的指令
    if(!strcmp(argv[0],"&")) return 1;
    //确认运行的返回值~
    return 0;
}

//执行命令行语句
void eval(char* cmdline){
    char* argv[MAX_ARGV_NUM];
    char buf[CMD_MAX_LENGTH];
    int isBackground;
    pid_t pid;

    strcpy(buf,cmdline);

    isBackground = parseLine(buf,argv);
    //文件名为空...
    if(argv[0] == NULL) return;

    if(!builtin_command(argv)){
        if((pid = fork()) == 0){
            if(execve(argv[0],argv,NULL)<0){
                printf("%s : Command Not Found.\n",argv[0]);
                exit(0);
            }
        }
        if(!isBackground){
            int status;
            if(waitpid(pid,&status,0)<0){
                printf("WaitPid Error\n");
            }
        }else{
            printf("%d , %s \n",pid,cmdline);
        }
    }
    return ;
}



int main(){
    char cmdline[CMD_MAX_LENGTH];
    while(1){
        printf("> ");
        gets(cmdline,CMD_MAX_LENGTH,stdin);
        if(feof(stdin)) exit(0);
        eval(cmdline);
    }
}