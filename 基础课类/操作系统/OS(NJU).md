# 操作系统

## 换个角度看操作系统

### 什么是操作系统？

​	操作系统首先是一个程序，它直接运行在硬件之上（因此需要实现很多的中断函数配置、完成很多与硬件的交互），操作系统之下的就是裸机（玩儿过的单片机等）。而操作系统之上，则是用户态应用程序，操作系统做到了以下几件事：

- 提供系统调用，使得上层应用能够通过OS与硬件进行**安全的交互**；本质上就是对下层硬件进行管理与抽象
- 对上层应用进行管理（即李治军老师课程里所说的“多进程视角”），操作系统通过时钟中断不断唤醒自己并进行进程的切换，以**提高CPU的利用率**

​	因此综合来看，OS是**管理计算机硬件与软件资源**的计算机**程序**。

### 什么是程序？

​	操作系统是程序，那么什么是程序？程序从本质上来看，就是CPU指令的一个序列，CPU能够读懂并执行这一系列指令，这就是程序

​	另一个角度来看，程序运行在数字系统之上，数字系统是一个状态机，那么程序也必然是一个状态机。问题在于，软件程序的状态是什么？又是怎么变化的？

- 状态就是计算机中所有硬件的状态。比如内存（堆、栈）、CPU中的寄存器等。
- 这些状态随着输入的指令变化

