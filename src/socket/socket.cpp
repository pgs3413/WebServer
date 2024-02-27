#include "socket.h"

Socket::Socket(int fd_) : fd(fd_) {
}

Socket::Socket(Socket &&socket){
    fd = socket.fd;
    port = socket.port;
    host = socket.host;
    socket.fd = -1;
}

Socket::Socket(const std::string &host_, unsigned short port_){
    
    fd = -1;
    int ret;

    port = htons(port_);
    ret = inet_pton(AF_INET, host_.c_str(), &host);
    if(ret != 1){
        throw std::logic_error("could not convert host");
    }

    ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1){
        throw std::runtime_error("could not create socket");
    }

    fd = ret;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr = host;

    ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
         throw std::runtime_error("could not connect");
    }

}

Socket::~Socket(){
    closeSocket();
}

void Socket::closeSocket(){
    if(fd >= 0){
        close(fd);
        fd = -1;
    }
}

Socket & Socket::operator=(Socket &&socket){
    closeSocket();
    fd = socket.fd;
    port = socket.port;
    host = socket.host;
    socket.fd = -1;
    return *this;
}

int Socket::readSocket(void *buf, size_t size){
    if(fd < 0){
        throw std::logic_error("socket not yet created");
    }
    int ret = read(fd, buf, size);
    if(ret < 0){
        throw std::runtime_error("socket could not read");
    }
    return ret;
}

int Socket::writeSocket(const void *buf, size_t size){
    if(fd < 0){
        throw std::logic_error("socket not yet created");
    }
    int ret = write(fd, buf, size);
    if(ret < 0){
        throw std::runtime_error("socket could not write");
    }
    return ret;
}