#ifndef TIMER_H_
#define TIMER_H_

#include<mutex>
#include<functional>
#include<chrono>
#include "../queue/smallheap.h"


class Timer {

private:

    typedef std::function<void()> Callback;
    typedef std::chrono::high_resolution_clock Clock;
    typedef Clock::time_point TimeStamp;
    typedef std::chrono::milliseconds MS;

    struct Item
    {
        int id_;
        TimeStamp expiration;
        Callback cb;
        
        int id(){
            return id_;
        }

        TimeStamp & priority(){
            return expiration;
        }

    };

    SmallHeap<Item> heap;
    std::mutex mtx;

public:

    Timer();
    ~Timer();

    void add(int id, int timeoutMs, Callback cb);
    void del(int id);
    void adjust(int id, int timeoutMs);
    int tick();

};

#endif