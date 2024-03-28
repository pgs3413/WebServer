#include "../src/pool/threadpool.h"
#include "../src/log/log.h"
#include<string>
#include<thread>

int main(){

    logger::init(logger::INFO, "", "", 1000, true, false);

    auto func = [](int second, const std::string &s){
        logger::info("{} start to work, need {} seconds...", s, second);
        std::this_thread::sleep_for(std::chrono::seconds(second));
        logger::info("{} finished work!", s);
    };

    ThreadPool threadPool(100, 3);

    threadPool.submit([&](){func(3,"task 1");});
    threadPool.submit([&](){func(2,"task 2");});
    threadPool.submit([&](){func(2,"task 3");});
    threadPool.submit([&](){func(1,"task 4");});

    threadPool.join();

    logger::info("all task finish!");
    
    return 0;

}