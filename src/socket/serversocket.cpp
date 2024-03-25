#include "socket.h"


ServerSocket::ServerSocket(unsigned short port_, bool reuseAddress, int rcvBufSize, int backlog) : _Socket(-1) {

    port = htons(port_);
    host.s_addr = htonl(INADDR_ANY);

    int ret;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1){
        throw std::runtime_error("could not create server socket");
    }

    fd = ret;

    if(reuseAddress){
        int optval = 1;
        ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
        if(ret == -1){
        throw std::runtime_error("could not set reuseAddress");
        }
    }

    if(rcvBufSize > 0){
        ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const void *)&rcvBufSize, sizeof(rcvBufSize));
        if(ret == -1){
            throw std::runtime_error("could not set rcvbuf");
        }
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = host;
    addr.sin_port = port;

    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        close(fd);
        fd = -1;
        throw std::runtime_error("could not bind to port");
    }

    ret = listen(fd, backlog);
    if(ret == -1){
        close(fd);
        fd = -1;
        throw std::runtime_error("could not listen");
    }

}

ServerSocket::~ServerSocket(){
    closeSocket();
}


Socket ServerSocket::acceptSocket(){
    if(fd < 0) {
        throw std::logic_error("server socket not yet created");
    }
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(addr);
    int socketFd = accept(fd, (struct sockaddr *)&addr, &addrlen);

    Socket socket(socketFd);
    if(socketFd >= 0){
        socket.port = addr.sin_port;
        socket.host = addr.sin_addr;
    }

    return socket;
}
