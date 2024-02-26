#include "../src/socket/socket.h"
#include<exception>
#include<iostream>

int main(){

    try{

        Socket client("127.0.0.1", 8080);

        std::string s;
        std::cin >> s;
        std::cout << "send " << s << " to server" << std::endl;

        client.writeSocket(s.c_str(), s.size());

        client.closeSocket();
        
    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}