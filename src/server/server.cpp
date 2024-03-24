#include "server.h"

void exitHander(int sig){
    info("------ server exited! ------");
    exit(0);
}

Server::Server(unsigned short port, int timeoutMs) :
port(port), timeoutMs(timeoutMs),
 serverSocket(nullptr)
{    
    info("server creating...");
    debug("port: {}, socket timeout: {}ms", port, timeoutMs);
    ServerSocket *temp;
    try {
        temp = new ServerSocket(port);
        info("server socket created.");
    }catch(std::exception &e){
        info("unable to create server socket!");
        debug("internal error: {}", e.what());
        std::perror(nullptr);
        return;
    }
    serverSocket = std::move(std::unique_ptr<ServerSocket>(temp));
    debug("set server socket to linger 2.");
    serverSocket -> setLinger(2);
    debug("set server socket to non block.");
    serverSocket -> setNonBlock();
    debug("add server socket to epoller. mode: IN LT");
    epoller.addFd(*serverSocket, true, false, false, false, false);
    debug("set router handlers.");
    http::Router::setDefaultHandler(_404Handler);
    http::Router::setErrHandler(_400Handler);
    http::Router::addHandler("/*", ResourceHandler);
    debug("set SIGINT handler for exit.");
    signal(SIGINT, exitHander);
    info("server created.");
}

Server::~Server(){
    if(serverSocket != nullptr){
        serverSocket -> closeSocket();
    }
}

void Server::start(){
    if(serverSocket == nullptr){
        info("server socket not yet created!");
        info("exited!");
        return;
    }
    info("------ server start ------");
    while (true)
    {
        int waitTimeMs = timer.tick();
        epoller.wait(waitTimeMs);
        for(auto event : epoller){
            if(event.getFd() == int(*serverSocket)){
                this -> accept();
            }
        }
    }
}

void Server::accept(){
    while (true)
    {
        Socket socket = serverSocket -> acceptSocket();
        if(socket < 0) break;
        assert(connMap.count(socket) == 0);
        http::Connction *conn = new http::Connction(std::move(socket));
        debug("接受到连接，对方地址:{}，对方端口:{}",conn->getAddress(), conn->getPort());
        conn -> init();
        epoller.addFd(*conn, true, false, true, false, true);
        timer.add(*conn, timeoutMs, std::bind(&Server::closeSocket, this, "连接超时", *conn));
        connMap[*conn] = std::move(std::unique_ptr<http::Connction>(conn)); 
    }
}

void Server::closeSocket(std::string reason, int fd){
    assert(connMap.count(fd) == 1);
    debug("因{}，连接断开，对方地址:{}，对方端口:{}", reason, connMap[fd]->getAddress(), connMap[fd]->getPort());
    epoller.delFd(fd);
    connMap.erase(fd);
}