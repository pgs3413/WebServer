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

bool _Socket::setLinger(int level){

    struct linger optval = {0};

    if(level == 1){
        optval.l_onoff = 1;
        optval.l_linger = 0;
    }else if(level == 2){
        optval.l_onoff = 1;
        optval.l_linger = 1;
    }

    int ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, (const void *)&optval, sizeof(optval));
    if(ret == -1) return false;
    return true;

}