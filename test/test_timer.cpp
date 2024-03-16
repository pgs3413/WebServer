#include "../src/timer/timer.h"
#include"../src/epoll/epoller.h"
#include<iostream>

static Timer timer;
static Epoller epoller;

auto getCallback(int id, int timeout){
    return [id,timeout](){
        std::cout << id << " expired after " << timeout << "ms" << std::endl; 
    };
};

void add(int id, int timeout){
    timer.add(id, timeout, getCallback(id,timeout));
}

int main(){

    add(1, 10);
    add(2, 1000);
    add(3, 3000);
    add(4, 5000);
    add(5, 4000);
    add(6, 2000);

    int time = -1;
    int round = 0;
    while ((time = timer.tick()) != -1)
    {
        round++;
        std::cout << "wait " << time << " ms..." << std::endl;
        epoller.wait(time);
        std::cout << "start next tick" << std::endl;

        if(round == 1){
            std::cout << "sub 1000ms to 4" << std::endl;
            timer.adjust(4, 4000);
        }

        if(round == 2){
            std::cout << "delete 5" << std::endl;
            timer.del(5);
        }

        if(round == 3){
            std::cout << "add 1000 ms to 3" << std::endl;
            timer.adjust(3, 4000);
        }

    }

    std::cout << "all done, goodbye." << std::endl;
    
   return 0; 

}