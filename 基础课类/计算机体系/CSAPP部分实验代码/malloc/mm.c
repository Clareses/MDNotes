#include "memlib.c"

#define WSIZE 4 //字长
#define DSIZE 8 //双字长
#define CHUNKSIZE (1<<12) //不懂，之后看

#define MAX(x,y) ((x)>(y)?:(x):(y))

#define PACK(size,alloc) ((size)|(alloc)) //方便头部或脚部打包

//读写一个字
#define GET(p) (*(unsigned int *)(p))
#define PUT(p,val) (*(unsigned int *)(p) = (val))

//从地址p中读取头部信息
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

//给一个首地址，计算出它的块的头部和脚部
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp))-DSIZE)

//给一个块地址，计算出下一个块或上一个块的首地址
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp)-DSIZE)))

static char* heap_listp;

static void* coalesce(void* bp){
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if(prev_alloc&&next_alloc) return bp;
    
    else if(prev_alloc&&!next_alloc){
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp),PACK(size,0));
        PUT(FTRP(bp),PACK(size,0));
    }else if(!prev_alloc&&next_alloc){
        size+= GET_SIZE(FTRP(PREV_BLKP(bp)));
        PUT(FTRP(bp),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }else{
        size += GET_SIZE(FTRP(PREV_BLKP(bp)))+GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

//释放空间
void mm_free(void* bp){
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    coalesce(bp);
}

//扩展words大小的空间
static void* extend_heap(size_t words){
    char* bp;
    size_t size;

    //算一个双字对齐的size
    size = (words%2)? (words+1)/WSIZE :words/WSIZE;
    if((long)(bp=mem_sbrk(size)) == -1) return NULL;

    //初始化块的头和脚
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
    //重新添加一个结尾的头
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));

    //顺便看看bp前后有没有可以合并的
    return coalesce(bp);
}

//初始化空闲链表
int mm_init(void){
    //创建空堆，并判断是否成功
    //四个字，前一是空的，中间俩是序言,最后一个是堆尾
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void*) -1){
        return -1;
    }
    //前一字写0,用来和堆尾凑双字对齐
    PUT(heap_listp,0);
    //中间俩字写序言的头和脚
    PUT(heap_listp+WSIZE,PACK(DSIZE,1));
    PUT(heap_listp+2*WSIZE,PACK(DSIZE,1));
    //堆尾写头
    PUT(heap_listp+3*WSIZE,PACK(0,1));
    //heaplistp放在序言中间
    heap_listp+=DSIZE;

    //为堆扩展CHUNKSIZE大小的空间 4MB
    if(extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;
    return 0;
}

//搜索合适的块
static void* find_fit(size_t size){
    char* bp = heap_listp;
    while(GET_SIZE(HDRP(bp)) != 0){
        if(GET_ALLOC(HDRP(bp))!=1&&GET_SIZE(HDRP(bp))>size)
            return bp;
    }
    return NULL;
}

static void place(void* bp,size_t asize){
    size_t oldsize = GET_SIZE(bp);
    PUT(HDRP(bp),PACK(asize,1));
    PUT(FTRP(bp),PACK(asize,1));
    PUT(HDRP(NEXT_BLKP(bp)),PACK(oldsize - asize),0);
    PUT(FTRP(NEXT_BLKP(bp)),PACK(oldsize-asize),0);
}

void* mm_malloc(size_t size){
    size_t asize;       //合适的size
    size_t extendsize;  //不合适的话需要扩展的size

    char* bp;

    if(size == 0) return NULL;

    if(size <= DSIZE){
        asize = 2*DSIZE;
    }else{
        asize = DSIZE*((size+2*DSIZE-1)/DSIZE);
    }

    if((bp = find_fit(asize))!= NULL){
        place(bp,asize);
        return bp;
    }

    extendsize = MAX(asize,CHUNKSIZE);
    if((bp = extend_heap(extendsize/WSIZE))==NULL){
        return NULL;
    }
    place(bp,asize);
    return bp;
}