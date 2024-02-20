#include "threadpool.h"

ThreadPool::ThreadPool(int queueCapacity, int threadNum) 
: threadNum(threadNum <= 0 ? 1 : threadNum) , 
  taskQueue(queueCapacity) , joinable(true)
{
    threadArray = new std::thread *[threadNum];
    for(int i = 0; i < threadNum; i++){
        threadArray[i] = new std::thread([this]{
            Func func;
            while (this -> taskQueue.pop(func))
            {
                func();
            }
        });
    }
}

bool ThreadPool::submit(Func func){
    return taskQueue.push(func);
}

void ThreadPool::join(){
    std::lock_guard<std::mutex> locker(mtx);
    if(!joinable) return ;
    joinable = false;
    taskQueue.close();
    for(int i = 0; i < threadNum; i++)
        threadArray[i] -> join();
}

ThreadPool::~ThreadPool(){
    join();
    delete [] threadArray;
}