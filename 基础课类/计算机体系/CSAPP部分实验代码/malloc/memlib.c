#define MAX_HEAP 1024

//私有的全局变量
static char* mem_heap;      //指向堆的第一个字节
static char* mem_brk;       //指向堆顶
static char* mem_max_addr;  //指向堆最大地址的后一位

//初始化内存系统模型
void mem_init(void){
    mem_heap = (char*)malloc(MAX_HEAP);
    mem_brk = (char*)mem_heap;
    mem_max_addr = (char*)mem_heap + MAX_HEAP;
}

//申请堆空间，返回起始地址
void* mem_sbrk(int incr){
    char* oldbrk = mem_brk;
    
    if(incr<0 || (mem_brk + incr)>mem_max_addr){
        return (void*)-1;
    }

    mem_brk += incr;
    return (void*)oldbrk; 
}