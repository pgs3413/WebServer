#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include<sys/socket.h>
#include<sys/ioctl.h>
#include<sys/uio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdexcept>
#include<string>

class _Socket {

protected:

    int fd;
    in_port_t port;
    struct in_addr host;

    _Socket(int fd);
    ~_Socket();

public:

    void closeSocket();
    bool setNonBlock(bool isNonBlock = true);
    bool setLinger(int level = 0);
    operator int();
    std::string getAddress();
    unsigned short getPort();

};

class Socket : public _Socket {
    
public:  

    friend class ServerSocket;
    explicit Socket(int fd_ = -1);
    Socket(const std::string &host_, unsigned short port_);
    Socket(const Socket &) = delete;
    Socket(Socket &&socket);
    ~Socket();
    
    Socket & operator=(const Socket &) = delete;
    Socket & operator=(Socket &&socket);

    int readSocket(void *buf, size_t size);
    int writeSocket(const void *buf, size_t size);
    int writevSocket(const struct iovec *iov, int iovcnt);

    int readableBytes();

};

class ServerSocket : public _Socket {


public:
    ServerSocket(unsigned short port_, bool reuseAddress = true, int rcvBufSize = -1, int backlog = 1000);
    ~ServerSocket();
    Socket acceptSocket();

};




#endif