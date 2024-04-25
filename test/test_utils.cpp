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


    // std::string key("dGhlIHNhbXBsZSBub25jZQ==");
    // key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    // std::string s = sha1AndBase64(key);
    // cout << s << endl; //s3pPLMBiTxaQ9kYGzzhZRbK+xOo=

    unsigned char payload[] = {'h','e','l','l','o'};
    unsigned char mask_key[] = {0x55, 0x12, 0xC3, 0x79};
    unsigned char des[5] = {0};

    mask(mask_key, 4, payload, des, 5);

    //3d77af153a
    for(int i = 0; i < 5; i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(des[i]);
    }

    std::cout << std::endl;

    unsigned char origin_payload[5] = {0};
    mask(mask_key, 4, des, origin_payload, 5);
    std::string s;
    for(int i = 0; i < 5; i++){
        s += origin_payload[i];
    }
    //hello
    std::cout << s << std::endl;


    return 0;

}