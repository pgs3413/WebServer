#include "../src/socket/socket.h"
#include<exception>
#include<iostream>

int main(){

    try{

        std::cout << "client connet to 8080..." << std::endl;

        Socket client("127.0.0.1", 8080);


        int writeBufSize = 0;
        int ret;
        // ret = setsockopt(client, SOL_SOCKET, SO_SNDBUF, (const void *)&writeBufSize, sizeof(int));
        // if(ret == -1){
        //     throw std::runtime_error("could not set sndbuf");
        // }

        socklen_t len = sizeof(int);
        ret = getsockopt(client, SOL_SOCKET, SO_SNDBUF, &writeBufSize, &len);
        if(ret == -1){
            throw std::runtime_error("could not get sndbuf");
        }

        std::cout << "client connet to 8080 successful " << "(sndbuf = " << writeBufSize << ")" << std::endl;

        std::string s;
        std::string ss;
        std::cin >> ss;
        int size ;
        std::cin >> size;
        for(int i = 0; i < size; i++){
            s += ss;
        }
        std::cout << "send " << s << " to server" << std::endl;

        client.writeSocket(s.c_str(), s.size());

        //测试四次挥手
        std::cin >> s;

        client.closeSocket();
        
    }catch(std::exception & e){
        std::cout << e.what() << std::endl;
    }

    std::perror(nullptr);

    return 0;
}