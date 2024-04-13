#include "../src/socket/socket.h"
#include<iostream>
#include<exception>
#include<cerrno>

int main(){

    try{

        ServerSocket server(8081, true, 1024);
        std::cout << "server wait to connet..." << std::endl;
        // server.setNonBlock();
        Socket socket = server.acceptSocket();
        if(socket == -1){
            // std::cout << "server won`t block..., change to block" << std::endl;
            // server.setNonBlock(false);
            // socket = server.acceptSocket();
            throw std::runtime_error("could not accept");
        }

        //设置接收缓冲区
        int ret;

        int rcvBufSize = 0;
        socklen_t len = sizeof(int);
        ret = getsockopt(socket, SOL_SOCKET, SO_RCVBUF, &rcvBufSize, &len);
        if(ret == -1){
            throw std::runtime_error("could not get rcvbuf");
        }

        
        std::cout << "server wait to read... " << "(rcvbuf = " << rcvBufSize << ")" << std::endl;
        std::string ch;
        std::cin >> ch;

        socket.setNonBlock();
        char buf[1024];
        std::string s;
        int bytes = 0;

        while ((bytes = socket.readSocket(buf, 1024)) > 0)
        {
            s.append(buf, bytes);
        }
        
        std::cout << "server read msg: " << s << std::endl;

        // 测试四次挥手
        std::cin >> s;

        socket.closeSocket();
        server.closeSocket();

    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
    }

    std::perror(nullptr);

    return 0;
}