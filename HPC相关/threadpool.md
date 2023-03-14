# 线程池

## std::thread

```c++
#include <thread>
#include <functional>

// 线程的基本控制
std::thread(Fn& fn, Args&&... args);
thread.join();
thread.detach();

thread.yield();
thread.get_id();
thread.native_handle();  // 获取posix风格的线程句柄

//锁与条件变量
std::mutex;
mutex.lock();
mutex.unlock();
mutex.try_lock();
使用unique_lock与lock_guard,上锁与解锁可以更灵活;
unique_lock<std::mutex>(mutex)会上锁;
在unique_lock对象销毁时，会自动解锁并解除与mutex的绑定;

std::condition_variable;
//第二个参数可以接收闭包，可以不用自己写while
condition_variable.wait(std::unique_lock, pred);
    
//关于闭包的使用
[capture](args)-> rettype {body}
```

## 线程池

### 什么是线程池？

​	线程池是多线程的一种处理形式，复用了已经创建的线程去执行任务，减少了线程创建和销毁的开销。线程池由一个管理者线程、多个用于执行任务的线程和一个任务队列共同构成，其中管理者线程负责检查线程池的状态（是否忙或是空闲）并根据状态去增加、减少工作线程；工作线程则不断运行一个处理函数，检查任务队列的情况来决定自身状态（从队列中取出任务执行或是睡眠）。

### 实现简单的线程池

```c++
#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <mutex>
#include <pthread.h>
#include <queue>
#include <thread>

class threadpool_t {
    // 考虑线程池需要对外提供的接口
  public:
    // constructor
    threadpool_t(std::size_t n) {
        this->waiting_task_num = 0;
        threads_list = new std::thread*[n];
        for (size_t i = 0; i < n; i++)
            threads_list[i] = new std::thread(thread_routine, this);
        this->thread_num = n;
    }

    // 添加任务
    void add_task(void* (*task)(void*), void* args) {
        {
            std::unique_lock<std::mutex>(this->mutex);
            {
                std::unique_lock<std::mutex>(this->waiting_mutex);
                this->waiting_task_num++;
            }
            task_queue.push((task_t){task, args});
            has_task.notify_one();
        }
    }

    // 获取剩余任务数
    size_t get_queue_size() { return waiting_task_num; }

    // destructor
    ~threadpool_t() {
        for (size_t i = 0; i < thread_num; i++){
            delete threads_list[i];
        }
        delete[] threads_list;
    }

  private:
    struct task_t {
        void* (*task)(void*);
        void* args;
    };
    
    // 辅助函数
    static void thread_routine(threadpool_t* self) {
        while (true) {
            task_t* task;
            {
                if(self->stop == true)
                    pthread_exit(NULL);
                //这里有并发的一个经典坑
                //即该线程被唤醒后并不意味着马上得到调度执行，
                //如果另一个消费者先得到调度并将任务取走执行，
                //等到调度回该线程时，就会出现无资源可用的问题，因此要用while
                //(或是使用第二个参数)
                std::unique_lock<std::mutex> lck(self->mutex);
                self->has_task.wait(
                    lck, [self] { return !self->task_queue.empty(); });
                //取出任务，释放锁
                task = &self->task_queue.front();
                self->task_queue.pop();
            }
            //执行任务
            task->task(task->args);
            {
                std::unique_lock<std::mutex>(self->waiting_mutex);
                self->waiting_task_num--;
            }
        }
    }

    // 考虑线程池需要的数据
    size_t thread_num;
    std::thread** threads_list;
    std::queue<task_t> task_queue;
    std::mutex mutex;
    std::condition_variable has_task;

    size_t waiting_task_num;
    std::mutex waiting_mutex;
};
```

## 使用线程池对小任务进行优化

