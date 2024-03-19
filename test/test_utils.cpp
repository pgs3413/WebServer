#include "../src/utils/stringutils.h"
#include<iostream>

int main(){

    std::string str = "&aaa&bbb&ccc&";

    std::vector<std::string> vec = split(str, '&');

    std::cout << vec.size() << std::endl;

    for(auto &s : vec){
        std::cout << s << std::endl;
    }

    return 0;

}