#ifndef STR_BLOCK_QUEUE_H
#define STR_BLOCK_QUEUE_H

#include<string>
#include<deque>
#include<mutex>
#include<condition_variable>

using std::string;

class StrBlockQueue{
private:
    
    std::deque<string> queue;
    int capacity;
    bool isClosed;

    std::mutex mtx;
    std::condition_variable condConsumer;
    std::condition_variable condProducer;

public:

    explicit StrBlockQueue(int maxCapacity);
    ~StrBlockQueue();

    bool push(const string &s);
    bool pop(string &s);
    void close();


};

#endif