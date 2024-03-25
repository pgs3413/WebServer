#ifndef SERVER_H
#define SERVER_H

#include "../log/log.h"
#include "../socket/socket.h"
#include "../epoll/epoller.h"
#include "../http/router.h"
#include "../http/connection.h"
#include "../handler/resourcehandler.h"
#include "../handler/syshandler.h"
#include "../timer/timer.h"
#include "../pool/threadpool.h"
#include<memory>
#include<exception>
#include<cerrno>
#include<signal.h>
#include<unordered_map>
#include<assert.h>


using log::debug;
using log::info;

class Server {

private:

    int timeoutMs;
    unsigned short port;
    std::unique_ptr<ServerSocket> serverSocket;
    Epoller epoller;
    Timer timer;
    ThreadPool threadPool;

    std::unordered_map<int, std::unique_ptr<http::Connction>> connMap;


    void accept();
    void close(std::string reason, int fd);
    void read(http::Connction *conn);
    void write(http::Connction *conn);

public:
    Server(unsigned short port, int threadNum = 20, int timeoutMs = 300000, int queueCapacity = 1000);
    ~Server();

    void start();

};

#endif