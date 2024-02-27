#include "socket.h"


ServerSocket::ServerSocket(unsigned short port_, int backlog) : port(port_) {

    listenFd = -1;

    int ret;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1){
        throw std::runtime_error("could not create server socket");
    }

    listenFd = ret;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    ret = bind(listenFd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        close(listenFd);
        listenFd = -1;
        throw std::runtime_error("could not bind to port");
    }

    ret = listen(listenFd, backlog);
    if(ret == -1){
        close(listenFd);
        listenFd = -1;
        throw std::runtime_error("could not listen");
    }

}

ServerSocket::~ServerSocket(){
    closeSocket();
}


Socket ServerSocket::acceptSocket(){
    if(listenFd < 0) {
        throw std::logic_error("server socket not yet created");
    }
    struct sockaddr_in addr;
    socklen_t addrlen;
    int fd = accept(listenFd, (struct sockaddr *)&addr, &addrlen);
    if(fd == -1){
        throw std::runtime_error("could not accept socket");
    }
    Socket socket(fd);
    return socket;
}

void ServerSocket::closeSocket(){
    if(listenFd >= 0) {
        close(listenFd);
        listenFd = -1;
    }
}