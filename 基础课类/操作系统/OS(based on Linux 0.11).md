# Operating System

## Overview

在CSAPP中，已经初步了解过了编写的代码是如何一步步加载到内存中执行的

> 经过以下阶段
>
> - 编译，得到汇编代码
> - 汇编，得到目标文件
> - 链接，得到可执行文件
> - 加载，通过虚拟内存系统，将磁盘上的可执行文件按页映射入物理内存中
> - 执行，CPU从内存中读指令，并进行运算等操作

但是仍然不足以解释为什么能在屏幕上输出“Hello world”

这中间还差了一环，在输出时，我们**通过系统级调用**，向标准输出设备文件输出，而这一部分，就是由操作系统提供的

这门课的**目的就是理解操作系统如何运作，通过对上层提供接口，从而协调多种硬件共同工作**。 

## System Setup

### 预备知识

#### 计算机启动过程

- 计算机在上电后，会做的第一件事就是**运行BIOS**，完成计算机内部的检查与初始化（设置一些寄存器的值等），这一过程**会将CS：IP设置为 0x07c0**，**并将磁盘中第一个扇区（每个扇区大小为512KB）读入**，这将是计算机将执行的第一个程序
- 通过汇编可以与主板上的硬件进行交互（**BIOS提供了一系列的中断操作**），用户编写的汇编代码可以通过调用中断操作硬件（操作系统也是这样协调硬件的）
- 磁盘的读取是以磁盘上的**扇区为单位进行读取**的，BIOS为磁盘读取提供了中断（INT13）

#### CPU的工作模式

CPU分为两种工作模式，**实模式与保护模式**，这两种模式之间的区别主要在于寻址方式的不同。

工作模式的选择依赖于一个**CR0寄存器**

<img src="../../_Images/image-20220515133053041.png" alt="image-20220515133053041" style="zoom:50%;" />

下面引用一些介绍

> ​	实模式出现于早期8088CPU时期。当时**由于CPU的性能有限**，一共**只有20位地址线**（所以地址空间只有1MB），以及8个16位的通用寄存器，以及4个16位的段寄存器。所以**为了能够通过这些16位的寄存器去构成20位的主存地址，必须采取一种特殊的方式**。当某个指令想要访问某个内存地址时，它通常需要用下面的这种格式来表示：
>
> 　**(段基址：段偏移量)**
>
> ​	其中第一个字段是段基址，它的值是由**段寄存器**提供的(一般来说，段寄存器有6种，分别为cs，ds，ss，es，fs，gs，这几种段寄存器都有自己的特殊意义
>
> 实模式的"实"更多地体现在**其地址是真实的物理地址**。
>
> ​	随着CPU的发展，CPU的地址线的个数也从原来的20根变为现在的32根，所以可以访问的内存空间也从1MB变为现在4GB，寄存器的位数也变为32位。所以实模式下的内存地址计算方式就已经不再适合了。所以就**引入了现在的保护模式**，实现更大空间的，更灵活也**更安全**的内存访问。
>
> ​	**在保护模式下，CPU的32条地址线全部有效**，可寻址高达4G字节的物理地址空间;  但是我们的**内存寻址方式还是得兼容老办法**(这也是没办法的，有时候是为了方便，有时候是一种无奈)，即(段基址：段偏移量)的表示方式。当然**此时CPU中的通用寄存器都要换成32位寄存器(除了段寄存器)来保证寄存器能访问所有的4GB空间**。
>
> ​	我们的**偏移值和实模式下是一样的**，就是变成了32位而已，**而段值仍旧是存放在原来16位的段寄存器中**，**但是这些段寄存器存放的却不再是段基址了**，毕竟之前说过实模式下寻址方式不安全，我们在保护模式下需要加一些限制，而这些限制可不是一个寄存器能够容纳的，于是我们**把这些关于内存段的限制信息放在一个叫做全局描述符表**(GDT)的结构里。**全局描述符表中含有一个个表项**，每一个表项称为**段描述符。**而段寄存器在保护模式下存放的便是相当于一个数组索引的东西，通过这个索引，可以找到对应的表项。段描述符存放了段基址、段界限、内存段类型属性(比如是数据段还是代码段,注意**一个段描述符只能用来定义一个内存段**)等许多属性,具体信息见下图：
>
> <img src="../../_Images/image-20220515132505570.png" alt="image-20220515132505570" style="zoom:67%;" />
>
> ​	全局描述符表位于内存中，需要用专门的寄存器指向它后， CPU 才知道它在哪里。这个专门的寄存器便是**GDTR**(一个48位的寄存器),专门用来存储 GDT 的内存地址及大小。

同样地，中断也不再是直接访问了，而是通过IDT（中断定义表）来定位到对应的中断函数（系统调用基于这个机制）

### bootsect.s

源码文件放在同一文件夹下

- BIOS执行后

![image-20220514193329533](../../_Images/image-20220514193329533.png)

- bootsect-1 复制了自己

![image-20220514193502562](../../_Images/image-20220514193502562.png)

- 读取setup

![image-20220514193835529](../../_Images/image-20220514193835529.png)

- 读system

![image-20220514194102737](../../_Images/image-20220514194102737.png)

**不难看出，bootsect只是读取了后续需要的所有文件...**

### setup.s

![image-20220515144018407](../../_Images/image-20220515144018407.png)

setup一方面通过BIOS中断读取了一些系统需要的**硬件信息**并写在0x90000开始的一段内存中（如扩展内存的大小、显卡信息、光标位置等），另一方面将0x10000～0x90000的内存复制到0x00000～0x80000，即**将system部分往前移动**。最后**开启保护模式，跳转进入system部分执行**。



### head.s

Linux makefile将head.s放置在了system部分的开头，因此setup执行后将执行这一部分

这一部分由于已经开启了保护模式，因此可以使用32bit汇编进行编程了（终于舒服了...）

**这一部分初始化了GBT表后，调用main函数，并在main中不断循环，不再退出**

![image-20220515153850090](../../_Images/image-20220515153850090.png)

进入main之前，先准备好了paging信息

进入main后，如果main发生了return，则会进入L6，开始死循环（死机）

**main中会进行一系列init，包括内存、中断、设备、时钟、CPU等**

### 总结

操作系统在启动时，主要进行的操作就是 **读系统** 与 **初始化参数**

- bootsect读入系统与setup
- setup对系统的位置做一系列整理，并获取一些参数
- head为进入系统作最后准备，加载页表等
- main加载操作系统的各个模块

## System Call

### C内联汇编基础

```c
asm ( assembler template
        : output operands                /* optional */
        : input operands                 /* optional */
        : list of clobbered registers    /* optional */
);

其中 assembler template 为汇编指令部分。
前两个冒号后面的是操作数（输出、输入）：第一个冒号将汇编模板与第一个输出操作数分开，第二个冒号将最后一个输出操作数与第一个输入操作数（如果有）分开。总结就是：不同类型的操作数使用 : 分隔，相同类型的操作数使用 , 分隔。
逗号分隔每个组中的操作数。
操作数的总数为 10。
寄存器名称前有两个 %，这有助于 GCC 区分操作数和寄存器。
如果没有输出操作数但有输入操作数，那么输出操作数前的冒号不能省
```

例如

```c
void test(){
    int a = 10;
    int b = 0;
    asm volatile(
        "movl %0, %%eax;"
        "add  $10, %%ead;"
        "movl %%eax, %1"
        :"=" (a) //将%0与变量a绑定，"="代表仅可读
        :"+" (b）//同理，"+"代表可读可写
    );
    //即 b = a + 10
}
```

### 什么是系统调用

​		系统调用是操作系统给其上层的软件提供的一系列接口，让上层应用通过系统调用**安全地操作硬件**

### 为什么要有系统调用

​		系统调用可以最大程度地**保证内存中内核部分与用户部分的隔离**，使得用户无法直接访问到内核中的数据。尤其是对于Linux这样的多用户操作系统而言，保证内核中的数据不被访问是十分必要的（网课中举的例子是：内核中保存了用户的账户和密码；内核中与硬件交接的部分比如显存等，如果可以访问则可以看到别人操作的内容）

### 如何实现系统调用

#### 如何隔离内核与用户

​		这一内容完全是由**硬件实现**的。CPU对内存的访问最终是**通过CS:IP寄存器实现**，因此，采用CS的最低两位来表示操作的权限（**0为内核态，3为用户态**），同时设置两个特殊的段寄存器（**CPL与DPL，current/dest priority level**），分别表示**当前所在段**（即由CS决定）和**目标段**（当前指令要访问的段，由当前指令决定）。在进行访存时，**如果CPL<DPL，则允许操作**（即当前所在段的权限高于目标段），由此实现了内核态与用户态的隔离。

（注意，**这里的CS指的是GDT中的表项**，在操作系统初始化时，内核部分的内存就已经通过GDT表项初始化为0了；而在给用户态分配时，只需要分配GDT表项最低两位为3的即可）

#### 提供接口（陷入内核）

​		而为了进入内核，计算机也特别**提供了一个中断（INT 0x80）**用来**实现特权等级的转换**，而内核可以利用这个中断，**实现用户态的函数，在函数中中断到内核态，并调用内核中实现的异常处理程序**

再理顺一下，内核在系统调用中做了这些事：	

- 实现了一个**用户态函数，用来切换特权级，并在用户态函数中调用了内核态的异常处理程序**（上层用户不用自己调用中断，而是调用内核提供的用户态函数）（换句话说…如果自己写的C程序中也调用了这个中断，是不是可以直接在内核里随便操作？底下的代码说明，调用int 80必须往寄存器中放入系统调用号…**通过中断依然仅可以调用写好的异常处理程序**）
- 基于BIOS提供的支持，实现了**操作硬件的异常控制程序**，用于与硬件进行交互

贴一个系统调用的代码

```c
//unistd.h
#define _syscall3(type, name, atype, a, btype, b, ctype, c)
type name(atype a, btype b, ctype c){
    long __res;
    asm volatile(
    	"int $0x80;"
        :"=a"(__res)
        :"0"(__NR_##name),//这里是将__NR_name传入寄存器
        				  //NR_name有宏定义，是系统调用号
        //将abc作为参数，与系统调用用到的寄存器绑定
        "b"((long)a),
        "c"((long)b),
        "d"((long)c)
    );
    if(__res >= 0) return (type)__res;
    errno = -__res;
    return -1;
}
```

```C
//write.c
#define __NR_write ...;
_syscall3(int, write, int, fd, const char*, buf, off_set, count)；

//将这个宏展开后
int write(int fd, const char* buf, off_set count){
    long __res;
    asm volatile(
    	"int $0x80;"
        :"=a"(__res)
        :"0"(__NR_write),
        "b"((long)fd),
        "c"((long)buf),
        "d"((long)count)
    );
    if(__res >= 0) return (int)__res;
    errno = -__res;
    return -1;
}
//这就是上文所说的系统提供的陷入内核的用户态函数
```

```c
//main.c
void sched_init(void){
    ...
    //在main中初始化时，就将0x80中断与一个system_call中断处理函数绑定了
    //也因此，上面int的是0x80
    set_system_gate(0x80, &system_call);
	...
}

int main(){
    ...
        sched_init();
}

//include/asm/system.h
#define set_system_gate(n, addr)
{
    _set_gate(&idt[n], 15, 3, addr);
}

#define _set_gate(gate_addr, type, dpl, addr)
{
    //注意，这里的dpl就是dest priority level
    //在设置中断处理函数时，中断处理函数是在内核中的，即dpl = 0
    //而这里将dpl设置成3，用户态就可以通过中断去调用system_call这个函数了
	asm(
        "movw %%dx,%%ax；" //dx(addr)的值移入ax
		"movw %0,%%dx；"	  //0x8000+(dpl<<13)+(type<<8) 移入dx
		"movl %%eax,%1；"  //ax的值移入gate_addr
		"movl %%edx,%2；"  //dx的值移入 gate_addr + 4	
		: 
		: "i" ((short) (0x8000+(dpl<<13)+(type<<8))), 
		"o" (*((char *) (gate_addr))), 
		"o" (*(4+(char *) (gate_addr))), 
		"d" ((char *) (addr)),
        "a" (0x00080000) //目的函数地址的高32位，根据地址格式可以发现
        			     //目的函数的特权级为0（即内核态），也是在这里
        				 //完成了用户态到内核态的转换
    )
}
// 完成后，IDT表项的前32位为目标函数的addr
// 后32位中含有了dpl与type的信息（其中dpl为3,但是IDT指向的是特权级为0的函数）
//main.c中进行这样的处理后，再次调用INT 0x80时，就会从IDT表中取出段地址等信息跳转到内核态执行了
```

```assembly
#linux/kernal/system_call.s
.global _system_call
_system_call:
	#eax中存的是系统调用号，因为 "INT 0x80"："a"(__res)
	cmpl $nr_system_calls-1, %eax 
	ja _bad_system_call	#调用号错误
	#保存各个段寄存器
	push %ds
	push %es
	push %fs
	pushl %edx
	pushl %ecx
	pushl %ebx
	#让内核可以得到用户数据
	movl $0x10,%edx		
	mov %dx,%ds
	mov %dx,%es
	#调用了 sys_call_table + %eax * 4 处的函数
	#因为每个函数占用4字节的空间，所以需要调用号*4得到函数所在的地址
	#这个函数就是异常处理函数了
	call sys_call_table(,%eax,4) 
	#将返回值压入栈中
	pushl %eax
	...
_ret_from_sys_call:
	...
	popl %eax
	iret
```

（sys_call_table，一个全局的表，记录了所有系统调用的函数指针）

![image-20220520165229606](../../_Images/image-20220520165229606.png)

#### 系统调用的完整流程

- 用户调用用户态函数
- 用户态函数**展开为一个包含了INT 0x80的函数**，并在这个函数内**根据函数名准备好了系统调用号**（__NR_name）
- INT 0x80在系统初始化时**绑定到一个system_call函数**上，并在这个**函数调用时完成用户态到内核态的转换**（本质上并没有发生转换，而是**利用中断将system_call这个位于内核态的函数暴露给用户**）
- system_call通过**系统调用号**与一个**全局系统调用表**调用到**位于内核态内的异常处理程序代码**（因为是system_call这个内核态函数调用的，因此可以直接访问异常处理程序）

## Process View—Overview

### 进程是什么

​	过去对进程的理解，停留在是**一个程序**的级别。而到操作系统这一层，进程的概念变得更为具体。一个进程不仅仅是要执行的程序的机器码，由于它需要进行切换，所以需要保存运行时的各种状态（PC的值、各个寄存器的值），因此进程更应该是**需要执行的机器码与操作系统管理它所需要的数据的集合体**。

​	由此可以**猜测操作系统是如何进行多进程切换**的：进程运行到一定位置，**触发一个中断从而陷入内核引起进程切换操作**（PC跳入虚拟内存的内核段中，执行内核的代码，这一部分很容易想到）。内核**更新维护的进程数据**，并**根据进程调度算法决定下一个要执行的进程**。切换理论上并不难，因为进程数据结构中维护了所需要的信息，为唤醒的进程恢复数据即可。但是问题出现在有虚拟内存，**操作系统给每个进程都提供了独占内存的假象，而CPU又通过虚拟内存来访问真实内存，那么操作系统如何处理虚拟内存**这一层隔离就成了一个问题（尤其是虚拟内存是一个软硬结合的产物，有专门的MMU和TLB来处理虚拟内存到物理内存的映射…）。

### 多个进程的组织

- **PCB（Process Control Block）**，进程控制的一种数据结构

- CPU的核心在执行某个进程（通过PCB中的数据）

- 利用PCB再组织起一个数据结构（列表等）

- **就绪队列**与**磁盘等待队列**：有些进程已经完成了IO操作，可以继续往下执行，这些进程构成就绪队列，CPU从这个队列中选择将要执行的进程；有些队列还未完成IO操作，仍然在等待与磁盘的交互，完成交互后再进入就绪队列

- 进程生命周期的状态转换图：

    <img src="../../_Images/image-20220521140140906.png" alt="image-20220521140140906" style="zoom: 33%;" />

### 调度算法的问题

- **FIFO**：经典的队列，先进先出地执行就绪态程序，但是没有考虑到**不同进程的性质问题**
- **优先级调度**：先执行优先级高的进程，等所有优先级高的进程完成后再执行低优先级的进程，但是可能**造成一些低优先级进程饥饿**
- （计网中学过的一种按优先级权重的FIFO）：划分时间片，每个时间片都将整个就绪态队列运行一遍，同时根据优先级决定每个进程占用时间片的长度。

### 内存管理与虚拟内存

​	为了防止多个进程之间对内存的访问互相干扰，**OS为每个进程都维护了一个内存映射表（页表），为每个进程都提供了独占内存的假象，使得多进程得以同时运行**。此外，虚拟内存还带来了一些意外的好处：**每个内存段的权限控制更加精确，使得破坏性程序无法操作到一些重要数据**。

​	虚拟内存的组织在CSAPP中其实也已经学过了一部分，主要是**通过数据结构来维护每个内存段的权限、物理地址等**。

​	这时，上面关于进程调度的一个问题似乎也可以解决了：操作系统多进程切换时如何“绕过”虚拟内存？只需要**设置MMU访问的页表位置**即可…页表一换，虚拟地址虽然还在同一个范围，但是访问到的物理地址已经不一样了，得到的指令自然也不一样了。

### 多进程间的通信与合作

​	进程之间也存在某些依赖关系，如编辑、保存、打印等。因此，进程间必须能进行通信，才能保证按合理的顺序完成工作

​	核心在于进程同步，通过锁机制等方式去强制执行序列趋于唯一，实现合理地推进程序。

### Process View的学习内容

- 读写**PCB**（**OS中最重要的结构，这一结构贯穿整个OS过程**）

- 需要写**汇编代码**去**实现调度程序中的寄存器保存、恢复等程序**
- 需要实现一些**调度算法，用来合理地管理进程队列**
- 需要**实现进程间的同步与合作**，实现进程间的通信等内容
- 需要**实现虚拟内存**，为每个进程**提供独占内存的假象**

## Process View—Process & Thread

### 线程 or 进程？

​	**线程**（Thread），可以认为是**轻量级的进程**，线程之间的切换也需要保存、恢复寄存器的值，但是由于线程在同一个进程内，进程可以**为线程分配固定的内存位置**，**而不需要像进程切换那样进行页表的转换**。因此，**线程具体来说，实现了多个执行序列与一个地址空间**。课程到这时候还没有学习虚拟内存的内容，因此将进程调度的重点放在进程切换时对寄存器、PC的处理，因此实际上就是在叙述线程的调度与切换。

### User Thread

#### 两个线程，一个栈？

```c
//假设有这样一段代码
Thread1{
    funcA(){
        funcB();
        return;
    }
    
    funcB(){
        Yield();
        return;
    }
}

Thread2{
    funcC(){
        funcD();
        return;
    }
    
    funcD(){
        Yield();
        return;
    }
}
```

​		假设代码从Thread1开始执行，关注栈的状态

![image-20220521204044398](../../_Images/image-20220521204044398.png)

​		当B返回时，控制流会根据栈返回到D中，控制流就乱了。因此，一个栈无法解决多个线程的调度问题，需要分配多个栈空间供不同的线程使用（同时记录每个栈的位置，这样利用内存就可以免去进程改页表的麻烦操作了）

#### 线程调度的设计

​	采取了多个栈的设计，并使用**全局变量TCB（Thread Control Block）描述每个线程的栈的栈顶位置**，并**在线程切换时将TCB读入RSP即可**（即上文所说的记录每个栈的位置）

```c
//模拟Yield函数的代码
//简化为由Thread1转到Thread2执行
void Yield_T1_to_T2(){
    TCB1.esp = esp;
    esp = TCB2.esp;
    return;
}
//为什么不需要修改PC的值？
//Thread2在之前调用Yield进入Thread1时，就需要将Yield后的代码压入栈中
//因此，在Thread1切换到Thread2的Yield函数返回时，ret出栈中的调用者的下一句指令就正好是Thread2 Yield之后的下一句指令（因为栈已经切换了～）
```

```c
//模拟ThreadCreate函数的代码
void Thread_Create(void* func){
    TCB* tcb = malloc();	//申请TCB的空间
    *stack = malloc();		//申请栈空间
    *stack = func;			//设置栈中的第一个代码位置
    tcb.esp = stack;		//记录栈顶
}
```

```c
//用户级Thread的实现
#include "stdio.h"

typedef struct Thread {
    __int64_t rsp;
    __int64_t stack[10000];
} Thread;

Thread thread1;
Thread thread2;

void createThread(void (*func)(), Thread* thread) {
    thread->rsp = (__int64_t)(thread->stack + 9998);
    thread->stack[9999] = (__int64_t)func;
}

void yield(Thread* threadSrc, Thread* threadDst) {
    __int64_t src_rsp;
    __int64_t dst_rsp;
    asm volatile("movq %%rsp, %0" : "=a"(src_rsp));
    threadSrc->rsp = src_rsp;
    dst_rsp = threadDst->rsp;
    asm volatile("movq %0, %%rsp" ::"a"(dst_rsp));
    return;
}

void funcA();
void funcB();
void funcC();
void funcD();

int main() {
    createThread(funcA, &thread1);
    createThread(funcC, &thread2);
    Thread mainThread;
    yield(&mainThread, &thread1);
}

void funcA() {
    printf("funcA Start\n");
    funcB();
}

void funcB() {
    printf("call funcB\n");
    while (1) {
        printf("funcB Yield\n");
        yield(&thread1, &thread2);
    }
}

void funcC() {
    printf("funcC Start\n");
    funcD();
}

void funcD() {
    printf("call funcD\n");
    while (1) {
        printf("funcD Yield\n");
        yield(&thread2, &thread1);
    }
}		
```

​	这种**用户级线程**为用户提供了线程切换、线程创建等函数，允许用户主动决定在什么位置切换线程。它**完全位于用户层，因此操作系统不会感知到它的存在**。但是同样地，如果这时**某个用户级线程进行了某个IO操作**，由于不知道它是多线程，**操作系统会进行进程切换**，从而**导致整个进程内的所有用户级线程都阻塞**。为了解决这个问题，**操作系统也提供了内核级线程，由内核维护TCB并进行调度**，就可以解决这个问题。

### Kernel Thread—Concept

​	下面进入内核级线程的部分。内核级线程主要是为了**协调多核CPU**而出现的，在Intel超线程技术出现以前，一个CPU核心就对应一条内核级线程（现在，单核CPU已经可以给操作系统提供自己的多核的假象了）。换句话说，**如果操作系统不支持Kernal Thread，那么多核CPU就无法发挥出作用**。

#### 多个核心共享内存？

​	多个核心，如何共享同一个物理内存空间？还是依赖了虚拟内存技术。一个CPU的**多个核心共享同一套Cache Memory和MMU**（核心级线程依然**被管理为同一个进程**，因此**使用的是同一个页表**，对内存的使用就与进程区别开来了）

#### 关于内核栈

​	内核要向用户进程隐藏细节，只提供系统调用。可是**系统调用也是过程**，并且也要进行过程调用；那么就一定**需要一个栈来存储调用过程时的数据与参数**等。**如果依然使用用户栈，那么用户代码可以轻松访问到系统调用遗留下来的数据**（退栈时不删除数据，只在压栈时覆写），因此引出了**内核栈**的概念。

​	在**陷入内核时（触发中断时，由硬件实现），操作系统要保存rsp当前的值与当前PC的值**（即用户栈的地址与当前执行程序的信息），然后将rsp切换到位于内核段中的一个内核栈中，**系统调用返回后，并不会保存内核栈的rsp**，而是直接丢弃（每次系统调用时，内核栈都是空的，因此也不需要保存）



#### 与用户线程的区别

​	正如上文提到的，**对于OS而言，用户级线程是不可见的**（用户态线程的概念很接近一些编程语言中的异步编程，本质上**代码并不是同时执行**，而是在进行切换），因此**同时最多只会有一个系统调用在发生**，并不需要对内核栈作出什么变化。而**内核态线程是真正意义上的多个核心在同时执行不同的代码，并且就算是单核，在阻塞时也会切换到别的内核态线程（这弥补了用户态线程会被当作整个进程切换掉的缺陷）**，因此是会同时发生多个系统调用的。因此，**给每个线程提供的不仅仅是多个用户栈，还需要提供多个内核栈**。

#### 内核态线程的切换

​	考虑在单核时的情况，当内核态线程阻塞时，也需要进行切换。内核态线程的切换则**完全由操作系统内核决定**了。

​	内核态线程切换只会发生在陷入内核时；当某个内核态线程陷入内核，如进行IO操作时，系统调用就会**调用switch_to**进行内核态线程的切换，寻找一个**同样也陷入内核中、但是已经完成了复杂的调用，可以返回用户态的线程**并切换。

> ​	内核态线程的切换主要就是内核栈的切换，操作系统维护着内核态线程的TCB，记录了内核栈的地址；切换时的操作与用户栈大同小异，唯一的区别在于，由于内核栈是由于中断引起的，返回时不能仅使用普通的ret，而需要一个iret从中断中返回。
>
> ​	这个区别主要还是因为栈的返回只需要执行的地址就行了，而栈的切换需要恢复数据。



![image-20220522135717854](../../_Images/image-20220522135717854.png)

**（当switch_to返回时，会ret出iret作为下一条指令，iret会从内核栈中恢复用户态信息后返回用户态）**

#### 内核态线程的生命周期

**五段论**

- 触发中断（可能是时钟，也可能是某些系统调用）
- 中断处理（调用了Schedule，引发切换）
- 调度算法（寻找下一个要切换的TCB）
- 内核栈切换（根据TCB切换线程）
- 中断出口（退出中断，回到用户态）

#### 内核线程的启动

​	与用户级线程一样，用户级线程是新建一个用户栈并将要执行的代码放在栈底，将TCB加入队列中。那么内核态线程就是，新建内核栈、用户栈，用户栈的底部放要执行的代码，内核栈的底部存用户栈信息、代码等，栈顶放上iret，并将存内核栈的TCB加入队列中等待调度。

### Kernel Thread—Implement

​	本节分析Linux中**fork系统调用**的代码，因为fork既触发了中断（导致线程切换），有涉及了线程的创建（新建进程需要主线程）

```c
//main.c

//首先复习syscall的相关内容
static inline int fork(void) {
    long __res;
    asm volatile("int $0x80" 	 	//调用0x80中断的处理程序
                 : "=a"(__res) 		//将rax作为输出与变量__res绑定
                 : "0"(__NR_fork)	//将系统调用号放入rax
                );
    if (__res >= 0)
        return (int)__res;
    errno = -__res;
    return -1;
}
```

```assembly
#system_call.s

#根据main.c中绑定的中断表，调用了中断处理程序
#int 0x80 - linux系统调用入口点(绑定中断int 0x80,eax中是调用号)
.align 2
system_call:
#首先判断是否是错误的系统调用号，如果错误，转入bad_sys_call中执行
	cmpl $nr_system_calls-1,%eax
	ja bad_sys_call
#将ds、es、fs压入栈中（这里已经切换到内核栈了，切换由硬件实现）
	
	push %ds		#内核栈的情况
	push %es		# SS：SP
	push %fs		# EFLAGS
	pushl %edx		# ins- mov res, %rax (int 0x80结束后的下一句)
	pushl %ecx		# ds（以下是保存用户态信息）
	pushl %ebx		# es
					# fs
					# edx
					# ecx
					# ebx
					
#为syscall准备参数
	movl $0x10,%edx
	mov %dx,%ds
	mov %dx,%es
#fs段寄存器用于访问用户空间的数据
	movl $0x17,%edx
	mov %dx,%fs
#根据sys_call_table决定了要调用的函数
	call sys_call_table(,%eax,4)
#得到结果，将eax存入栈中（eax接下去还有用）
	pushl %eax
##重点在这里（这是current就是TCB）
	movl current,%eax
#将TCB的state与0比较（如果不为0,说明阻塞，需要调度）
	cmpl $0,state(%eax)
	jne reschedule
#将TCB的counter与0比较（如果相等，说明时间片用完了，需要调度）
	cmpl $0,counter(%eax)
	je reschedule

#system_call执行后，就是这一段程序
ret_from_sys_call:
#这里有大段信号处理内容，先跳过，关注3
	movl current,%eax
	cmpl task,%eax
	je 3f
	cmpw $0x0f,CS(%esp)
	jne 3f
	cmpw $0x17,OLDSS(%esp)
	jne 3f
	movl signal(%eax),%ebx
	movl blocked(%eax),%ecx
	notl %ecx
	andl %ebx,%ecx
	bsfl %ecx,%ecx
	je 3f
	btrl %ecx,%ebx
	movl %ebx,signal(%eax)
	incl %ecx
	pushl %ecx
	call do_signal
	popl %eax
#从这里开始，是中断退出的部分
#一系列pop过后，内核栈回到下面的样子
3:	popl %eax			#内核栈情况
	popl %ebx			# SS
	popl %ecx			# SP
	popl %edx			#EFLAG
	pop %fs				# CS
	pop %es				# IP
	pop %ds
#执行iret,会回到用户栈
	iret
	
#重调度
reschedule:
	pushl ret_from_sys_call
	call schedule

```

#### Schedule

```c
// sched.c
void schedule(void) {
    //大部分涉及调度算法，先看最后的switch_to
    int i, next, c;
    struct task_struct** p;
//check alarm,wake up any interruptible tasks that have got a signal
    for (p = &LAST_TASK; p > &FIRST_TASK; --p){
        if (*p) {
            if ((*p)->alarm && (*p)->alarm < jiffies) {
                (*p)->signal |= (1 << (SIGALRM - 1));
                (*p)->alarm = 0;
            }
            if (((*p)->signal & ~(_BLOCKABLE & (*p)->blocked)) 
                &&(*p)->state == TASK_INTERRUPTIBLE)
                (*p)->state = TASK_RUNNING;
        }
    }
    while (1) {
        c = -1;
        next = 0;
        i = NR_TASKS;
        p = &task[NR_TASKS];
        while (--i) {
            if (!*--p)
                continue;
            if ((*p)->state == TASK_RUNNING && (*p)->counter > c)
                c = (*p)->counter, next = i;
        }
        if (c) break;
        for (p = &LAST_TASK; p > &FIRST_TASK; --p){
            if (*p){
                (*p)->counter = 
                	((*p)->counter >> 1) + (*p)->priority;
            }
        }
    }
    //实现了任务的切换
    switch_to(next);  // 切换到Next任务并运行。
}

//switch_to的宏
#define switch_to(n) {
//Linux 0.11的切换还是基于TSS的，本质上是拿一整个TSS结构记录了当前CPU执行的任务的所有寄存器的状态，使用一个专门的TR段寄存器存当前任务的选择子，通过找专门的内存得到当前任务的信息。
//TSS的基本思路就是，通过TR寄存器找到当前的TSS,并存入当前的所有寄存器数据；再修改TR,并将需要转到的任务的TSS恢复入寄存器中，这样就完成了栈的切换；这些过程会在线程执行clts时自动完成
	struct {
		long a, b;
	} __tmp;

	asm(
        "cmpl %%ecx,currentnt"
		"je 1fnt"
		"movw %%dx,%1nt"
		"xchgl %%ecx,currentnt"
		"ljmp *%0nt"	//TSS方式中最关键的指令（远址跳转）
		"cmpl %%ecx,last_task_used_mathnt"
		"jne 1fnt"
		"cltsn"	//调整CPU中的所有寄存器，从而实现栈切换
		"1:" 
        :
        :"m"(*&__tmp.a),
		"m"(*&__tmp.b),
		"d"(_TSS(n)),
		"c"((long)task[n])
	);
}

```

#### Create

```assembly
#从call sys_call_table(,%eax,4) 接着往下看
.align 2
sys_fork:
	call find_empty_process 	#寻找可用进程号
	testl %eax,%eax
	js 1f
	push %gs		#压了一些寄存器（猜测是在准备参数）
	pushl %esi
	pushl %edi
	pushl %ebp
	pushl %eax
	call copy_process 	#调用了copy_process
	addl $20,%esp
1:	ret
```

```c
//fork.c
int copy_process(int nr..../*一系列寄存器*/){
    struct task_struct *p;
	...;
    //核心代码，获取一块新的内存空间
	p = (struct task_struct *) get_free_page();
	if (!p) return -EAGAIN;
	task[nr] = p;
    *p = *current;	//复制父进程的TSS
    //对父进程的TSS进行一些修改
    //注意这里对父进程的内核栈进行了修改，但是用户栈仍然共用
    ...;
    p->tss.eax = 0; //这里给子进程的tss-eax设置成0,因此子进程能返回0
    ...;
    //重新设置TR段寄存器等
    set_tss_desc(gdt+(nr<<1)+FIRST_TSS_ENTRY,&(p->tss));
	set_ldt_desc(gdt+(nr<<1)+FIRST_LDT_ENTRY,&(p->ldt));
    return last_pid;//这里的last_pid是调用find_empty_process得到的
}

//父进程会执行代码，直到set_tss为止
//而子进程的所有信息都由父进程生成，并在set_tss后会正式开始运行，因此也会执行return语句，因此返回的是上面父进程设置的pid，即0
//父进程在得到调度后，会继续执行，因此也会return,但是由于并没有修改过自己的pid,因此会返回获取到的last_pid
```

##  Process View—Process Schedule

## Process View—IPC

## Process View—VM System

## File View—Driver

## File View—File System
