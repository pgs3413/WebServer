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

    // std::string s1("-12345");
    // std::string s2("123.1av");

    // cout << toNumber(s1, 0) << endl;
    // cout << toNumber(s2, 123) << endl;

    // std::string s = sha1AndBase64("abc"); //a9993e364706816aba3e25717850c26c9cd0d89d
    // cout << s << endl;

    std::string key("ERYsOLGW0FbU5wt+8kn4NA==");
    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string s = sha1AndBase64(key);
    cout << s << endl;

    return 0;

}