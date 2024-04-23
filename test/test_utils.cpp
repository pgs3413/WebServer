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


    std::string key("dGhlIHNhbXBsZSBub25jZQ==");
    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string s = sha1AndBase64(key);
    cout << s << endl; //s3pPLMBiTxaQ9kYGzzhZRbK+xOo=

    return 0;

}