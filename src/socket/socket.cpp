#include "socket.h"

Socket::Socket(int fd) : _Socket(fd) {
}

Socket::Socket(Socket &&socket) : _Socket(-1) {
    fd = socket.fd;
    port = socket.port;
    host = socket.host;
    socket.fd = -1;
}

Socket::Socket(const std::string &host_, unsigned short port_) : _Socket(-1) {
     
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
}

Socket & Socket::operator=(Socket &&socket){
    closeSocket();
    fd = socket.fd;
    port = socket.port;
    host = socket.host;
    socket.fd = -1;
    return *this;
}

Socket::operator int(){
    return fd;
}

int Socket::readSocket(void *buf, size_t size){
    if(fd < 0){
        throw std::logic_error("socket not yet created");
    }
    return read(fd, buf, size);
}

int Socket::writeSocket(const void *buf, size_t size){
    if(fd < 0){
        throw std::logic_error("socket not yet created");
    }
    return write(fd, buf, size);
}


int Socket::writevSocket(const struct iovec *iov, int iovcnt){
    if(fd < 0){
        throw std::logic_error("socket not yet created");
    }
    return writev(fd, iov, iovcnt);
}