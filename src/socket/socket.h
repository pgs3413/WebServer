#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdexcept>
#include<string>

class Socket{

private:
    int fd;
    in_port_t port;
    struct in_addr host;

public:
    explicit Socket(int fd_ = -1);
    Socket(const std::string &host_, unsigned short port_);
    Socket(const Socket &) = delete;
    Socket(Socket &&socket);
    ~Socket();
    
    Socket & operator=(const Socket &) = delete;
    Socket & operator=(Socket &&socket);

    int readSocket(void *buf, size_t size);
    int writeSocket(const void *buf, size_t size);
    void closeSocket();

};

class ServerSocket{

private:
    int listenFd;
    unsigned short port;

public:
    ServerSocket(unsigned short port_, int backlog = 1000);
    ~ServerSocket();
    Socket acceptSocket();
    void closeSocket();

};


#endif