#include "../src/utils/stringutils.h"
#include<iostream>

using namespace std;

int main(){

    // std::string str = "&aaa&bbb&ccc&";

    // std::vector<std::string> vec = split(str, '&');

    // std::cout << vec.size() << std::endl;

    // for(auto &s : vec){
    //     std::cout << s << std::endl;
    // }

    std::string s1("-12345");
    std::string s2("123.1av");

    cout << toNumber(s1, 0) << endl;
    cout << toNumber(s2, 123) << endl;

    return 0;

}