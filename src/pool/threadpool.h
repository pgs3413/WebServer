#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include"../queue/blockqueue.h"
#include<functional>
#include<mutex>
#include<thread>

class ThreadPool{

private:

    typedef std::function<void()> Func;

    int threadNum;
    BlockQueue<Func> taskQueue;
    std::mutex mtx;
    bool joinable;
    std::thread ** threadArray;


public:
    ThreadPool(int queueCapacity = 10, int threadNum = 1);
    ~ThreadPool();
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool & operator=(const ThreadPool &) = delete;
    bool submit(Func func);
    void join();

};

#endif