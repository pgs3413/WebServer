#include "socket.h"

_Socket::_Socket(int fd) : fd(fd){

}

_Socket::~_Socket(){

    closeSocket();

}

void _Socket::closeSocket(){
    if(fd >= 0){
        close(fd);
        fd = -1;
    }
}

bool _Socket::setNonBlock(bool isNonBlock){
    if(fd < 0) {
        throw std::logic_error("socket not yet created");
    }
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1) return false;

    if(isNonBlock){
        flags |= O_NONBLOCK;
    }else{
        flags &= ~O_NONBLOCK;
    }

    if(fcntl(fd, F_SETFL, flags) == -1) return false;
    return true;
}