#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H


#include<deque>
#include<mutex>
#include<condition_variable>

template<typename T>
class BlockQueue{
private:
    
    std::deque<T> queue;
    int capacity;
    bool isClosed;

    std::mutex mtx;
    std::condition_variable condConsumer;
    std::condition_variable condProducer;

public:

    explicit BlockQueue(int maxCapacity);
    ~BlockQueue() = default;

    bool push(const T &s);
    bool pop(T &s);
    void close();


};

template<typename T>
BlockQueue<T>::BlockQueue(int maxCapacity){
    capacity = maxCapacity <= 0 ? 1 : maxCapacity;
    isClosed = false;
}


template<typename T> 
bool BlockQueue<T>::push(const T &item){
    std::unique_lock<std::mutex> locker(mtx);
    while (!isClosed && queue.size() >= capacity){
        condProducer.wait(locker);
    }
    if(isClosed) return false;
    queue.push_back(item);
    condConsumer.notify_one();
    return true;
}
 
template<typename T>
bool BlockQueue<T>::pop(T &item){
    std::unique_lock<std::mutex> locker(mtx);
    while (!isClosed && queue.empty()){
        condConsumer.wait(locker);
    }
    if(isClosed && queue.empty()) return false;
    item = queue.front();
    queue.pop_front();
    condProducer.notify_one();
    return true;
}

template<typename T>
void BlockQueue<T>::close(){
    std::lock_guard<std::mutex> locker(mtx);
    isClosed = true;
    condProducer.notify_all();
    condConsumer.notify_all();
}

#endif