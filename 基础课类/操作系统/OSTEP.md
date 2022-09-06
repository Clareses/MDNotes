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

### Design Goals

-   Build up some **abstractions** in order to make the system **convenient and easy to use**
-   Provide **high performance**, **minimize the overheads** of the OS. Try to **make a balance** between features and performance.

-   Provide **protection** between applications, **isolating processes** to each other

-   Provide a **high degree of reliability**

## Part I  -- Virtualization

### The Abstraction: The Process

#### What is Process

​	What is a **Process**? it is **a running program** instead of siting on the disk. Also, the abstraction of process make OS can virtualize many CPUs. OS implement this illusion by **running one process, then stopping it and running another one, and so forth**. The Processes share the CPU in turns, which known as **time sharing**, allow users to run more than one program.

​	The potential cost is performance, which means that **OS should not take too much computing-power to implement it**.

​	To implement it, OS should provide supports not only **from the machine-level** (which called **context-switch**), but **from the high-level** (algorithm to finish **scheduling**).

#### How to describe a Process in Machine-Level

In total, **all the hard-wares** of a computer system decide the process, but who are they ?

-   **Memory**   All the instructions and the datas of a process are lie in the memory

-   **Register**   Registers are the interface of the CPU, which describe the CPU 's state (especially some special registers such as Program Counter, Stack Pointer, Frame Pointer and so on)

-   **Devices**    programs often acess persistent storage devices too. (especially the I/O system)

#### Life-Cycle of the Process

-   **Creation**    The *loader* in OS read the program in the disk and try to *map them into a correct position in memory* (not have to load all the pages at one time, with a VM System, OS can swap the required pages into memory when neccesarily, which called lazily load). After that, OS will call some function to do some prepare for process (such as run-time stack or heap). Also, OS do some initialization task like related to I/O. (Open some file descriptor)

>   **How OS prepare run-time stack and heap for process ?**
>
>   ​	When create a process, OS will record a PCB (process control block) structure in the kernel, the PCB maintain some variables to control the process.
>
>   ​	The run-time stack pointer is in the structure (stack means kernel stack but not user stack, in fact, user stack is not control by OS), when a process is running, it will be set in a register called TSS, which is a container of the running process infomation. When program yield into the kernel, hardware will store the sp (recording user stack now) and some other registers into the kernel stack, and then set the kernel stack into the sp to finish the switch between the user stack and the kernel stack. 
>
>   ​	For heap, OS maintain a variable called program break (which point to the end of the data segment) for each process to record the allocated heap space.
>
>   
>
>   **The file model in Linux**
>
>   ![image-20220906231750687](../../_Images/image-20220906231750687.png)
>
>   -   **FD Table**  Each process has a file descriptor table, whose item contain a fd and a pointer point to the table maintain the file read or write info.
>   -   **Open File Table**  Items in this table record the read or write info of each IO operation. Include data like refer counter (how many fd pointer to it, which means how many fd shared the IO info) and read write pointer record the file length has been read or write.
>   -   **v-node Table**  Each item in this table mapping a file physically in the disk, which contain the information about the file, such as file size, file type and so on.
>
>   **SO WHY NEED FD ?**
>
>   ​	Why OS don't provides the true address of each system call but provides the system call number to call these function? That is OS want to isolate the user code and the kernel code, just provides limited actions to user. The File System use file descriptor to acheive the same target.

-   **Ready**    After creation, the kernel stack has been pretend to be just like every yield process (has pc, user stack pointer, and some registers). In this state, the new creating process has been ready for being scheduled to run. Once scheduler choose it, OS will resume it context to CPU, and starting to execute the code of it.
-   **Running**    The scheduler choose a yield process, the process start runing ...
-   **Blocked**    The running process may take some action which need a lot of times. When doing these actions, the CPU are stopping to wait for it. So OS can switch to another process in ready state to run.

![image-20220906234756241](../../_Images/image-20220906234756241.png)

#### Data Structure of Process

```c
struct context {
	int eip;
    int esp;
    int ebx;
    int ecx;
    int edx;
    int esi;
    int edi;
    int ebp;
};

enum proc_state {
    UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE,
};

struct proc {
  	char* mem;
    uint32 memsz;
    char* kstack;
    
    enum proc_state state;
    int pid;
    struct proc* parent;
    void* chan;
    int killed;
    struct file *ofile[NOFILE];
    struct inode* cwd;
    struct context context;
    struct trapframe* tf;
};
```

### Mechanism: Limited Direct Execution
