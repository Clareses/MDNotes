#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "errno.h"

#define N 2

int main(){
    execve("executable",NULL,NULL);
}