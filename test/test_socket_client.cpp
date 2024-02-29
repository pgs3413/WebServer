#include "../src/socket/socket.h"
#include<exception>
#include<iostream>

int main(){

    try{

        std::cout << "client connet to 8080..." << std::endl;

        Socket client("127.0.0.1", 8080);

        std::cout << "client connet to 8080 successful" << std::endl;

        std::string s;
        std::cin >> s;
        std::cout << "send " << s << " to server" << std::endl;

        client.writeSocket(s.c_str(), s.size());

        //测试四次挥手
        std::cin >> s;

        client.closeSocket();
        
    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}