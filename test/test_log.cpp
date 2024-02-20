#include "../src/log/log.h"

using log::info;

int main(){

    log::init(log::INFO, "./",".log", 1000, true, true);
    std::string name("pan");
    info("name:{}, age:{}, address:{}, c:{}", name, 18, "China", 'a');
    log::debug("debug wouldn`t log");
    log::warn("name:{}, age:{}, address:{}, c:{}", name, 18, "China");
    log::error("name:{}, age:{}, address:{}, c:{}", name, 18, "China", 'a' , 20);

    return 0;
}