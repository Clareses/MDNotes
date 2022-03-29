#include "unistd.h"
#include "stdio.h"
#include "signal.h"

void sigint_handler(){
    printf("catch signal!\n");
    sleep(10);
    exit(0);
}

void sigtstp_handler(){
    printf("catch signal!\n");
    exit(0);
}

int main(){
    if(signal(SIGINT,sigint_handler) == SIG_ERR)
        printf("signal error\n");
    if(signal(SIGTSTP,sigtstp_handler) == SIG_ERR)
        printf("signal error\n");
    pause();
    return 0;
}