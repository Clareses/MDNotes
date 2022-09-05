# Operating System - Three Easy Pieces

## Overview

### What happened when a program run?

- **Fetch**
- **Decodes**
- **Executes**
- **Memory Access**
- **Write Back**

### What role the OS play in these stage?

- **Virtualize the physical resource**  OS take physical resource and transform them into a more *general*, *powerful*, and *easy-to-use* virtual form of itself (Take CPU for example, Virtualization can make it be used in a more efficient way by *Time Sharing*). Thus, we sometimes refer to the OS as a *virtual machine*.
- **Provides APIs to allow using the features of VM**  OS provides some interface called *"System Call"* to make the PC become easier and safer to be used. So we sometimes say that OS provides a *standard library* to applications.
- **Manages resource for applications above it**  OS also play a resource manager role in the computer system, it provides mechanism to make applications access devices in a correct order instead of conflicting.

### Embodiment of some mechanisms

#### Virtualizing the CPU

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#include "common.h"

int main(int argc, char* argv[]){
    char* str = argv[1];
    while(1) {
 		// the Spin(1) function will Block 1 second
        Spin(1);
        printf("%s\n", str);
    }
    return 0;
}
```

​	When execute beyond one instances of this Program, these instances will execute at the same time. *But how can **ONE CPU** execute **MORE THAN ONE PROGRAM AT THE SAME TIME**？*

​	OS provides a *Illusion* to each process that it holds the CPU all the time.

#### Virtualizing Memory

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int *p = (int*)malloc(sizeof(int));
    printf("Allocate a space at %p\n", p);
    while(1) {
        spin(1);
        printf("*p = %d\n", ++(*p));
    }
    return 0;
}
```

​	When execute two instances of this Program, you will find that the p of two processes are equal, but the *p values are independent with each other.  *The question is that why two processes **ACCESS THE SAME ADDRESS** of memory but **GET DIFFERENT VALUE**？* 

#### Concurrency

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0;

int loops;

void *worker(void* args) {
    for(int i = 0; i < loops; i++) {
        counter++;
    }
    return NULL;
}

int main(){
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value: %d\n", counter);
    
    pthread_create(&p1, NULL, worker, NULL);
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    
    printf("Final value: %d\n", counter);
    
    return 0;
}
```

​	In a normal view, this program will output final value which equals twice of loops. However, the results are not only not as expected, but even different everytime. **HOW IT OCCURED？**
