#include "../src/log/log.h"

using logger::info;

int main(){

    logger::init(loggger::INFO, "./",".log", 1000, true, true);
    std::string name("pan");
    info("name:{}, age:{}, address:{}, c:{}", name, 18, "China", 'a');
    logger::debug("debug wouldn`t log");
    logger::warn("name:{}, age:{}, address:{}, c:{}", name, 18, "China");
    logger::error("name:{}, age:{}, address:{}, c:{}", name, 18, "China", 'a' , 20);

    return 0;
}