#include "../src/socket/socket.h"
#include<iostream>
#include<exception>
#include<cerrno>

int main(){

    try{

        ServerSocket server(8080);
        std::cout << "server wait to connet..." << std::endl;
        // server.setNonBlock();
        Socket socket = server.acceptSocket();
        if(socket == -1){
            std::cout << "server won`t block..., change to block" << std::endl;
            server.setNonBlock(false);
            socket = server.acceptSocket();
        }

        char buf[1024];
        std::cout << "server wait to read..." << std::endl;
        int bytes = socket.readSocket(buf, 1024);
        std::string s(buf, bytes);

        std::cout << "server read msg: " << s << std::endl;

        // 测试四次挥手
        std::cin >> s;

        socket.closeSocket();
        server.closeSocket();

    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
        std::perror(nullptr);
    }

    return 0;
}