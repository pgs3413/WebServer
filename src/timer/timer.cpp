#include "timer.h"

Timer::Timer(){

}

Timer::~Timer(){

}

void Timer::add(int id, int timeoutMs, Callback cb){
    heap.add({id, Clock::now() + MS(timeoutMs), cb});
}
    
    
void Timer::del(int id){
    heap.del(id);
}


void Timer::adjust(int id, int timeoutMs){
    heap.adjust(id, Clock::now() + MS(timeoutMs));
}

int Timer::tick(){
    int time = -1;
    int size = heap.size();
    for(int i = 0; i < size; i++){
        Item item = heap.front();
        int duration = std::chrono::duration_cast<MS>(item.expiration - Clock::now()).count();
        if(duration <= 0){
            heap.pop();
            item.cb();
        }else{
            time = duration;
            break;
        }
    }
    return time;
}