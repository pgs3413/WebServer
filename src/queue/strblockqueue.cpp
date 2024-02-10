#include "strblockqueue.h"

StrBlockQueue::StrBlockQueue(int maxCapacity){
    capacity = maxCapacity <= 0 ? 1 : maxCapacity;
    isClosed = false;
}

StrBlockQueue::~StrBlockQueue(){

}
 
bool StrBlockQueue::push(const string &s){
    std::unique_lock<std::mutex> locker(mtx);
    while (!isClosed && queue.size() >= capacity){
        condProducer.wait(locker);
    }
    if(isClosed) return false;
    queue.push_back(s);
    condConsumer.notify_one();
    return true;
}
 
bool StrBlockQueue::pop(string &s){
    std::unique_lock<std::mutex> locker(mtx);
    while (!isClosed && queue.empty()){
        condConsumer.wait(locker);
    }
    if(isClosed && queue.empty()) return false;
    s = queue.front();
    queue.pop_front();
    condProducer.notify_one();
    return true;
}

void StrBlockQueue::close(){
    std::lock_guard<std::mutex> locker(mtx);
    isClosed = true;
    condProducer.notify_all();
    condConsumer.notify_all();
}