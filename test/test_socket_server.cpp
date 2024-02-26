#include "../src/socket/socket.h"
#include<iostream>
#include<exception>
#include<cerrno>

int main(){

    try{

        ServerSocket server(8080);
        std::cout << "server wait to connet..." << std::endl;
        Socket socket = server.acceptSocket();

        char buf[1024];
        std::cout << "server wait to read..." << std::endl;
        int bytes = socket.readSocket(buf, 1024);
        std::string s(buf, bytes);

        std::cout << "server read msg: " << s << std::endl;

        socket.closeSocket();
        server.closeSocket();

    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
        std::perror(nullptr);
    }

    return 0;
}