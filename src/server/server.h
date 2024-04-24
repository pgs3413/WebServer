#ifndef SERVER_H
#define SERVER_H

#include "../log/log.h"
#include "../socket/socket.h"
#include "../epoll/epoller.h"
#include "../http/router.h"
#include "../http/connection.h"
#include "../handler/resourcehandler.h"
#include "../handler/syshandler.h"
#include"../handler/cgihandler.h"
#include "../timer/timer.h"
#include "../pool/threadpool.h"
#include<memory>
#include<exception>
#include<cerrno>
#include<signal.h>
#include<unordered_map>
#include<assert.h>


using logger::debug;
using logger::info;

class Server {

private:

    int timeoutMs;
    unsigned short port;
    std::unique_ptr<ServerSocket> serverSocket;
    Epoller epoller;
    Timer timer;
    ThreadPool threadPool;

    std::unordered_map<int, std::unique_ptr<http::Connection>> connMap;


    void accept();
    void close(std::string reason, int fd);
    void read(http::Connection *conn);
    void write(http::Connection *conn);

public:
    Server(unsigned short port, int threadNum = 20, int timeoutMs = 300000, int queueCapacity = 1000);
    ~Server();

    void start();

};

#endif