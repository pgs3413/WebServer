#ifndef WEBSOCKET_WEBSOCKET_H
#define WEBSOCKET_WEBSOCKET_H

#include"../socket/socket.h"
#include"../http/request.h"
#include"../http/response.h"
#include"../utils/stringutils.h"
#include"../epoll/epoller.h"
#include"frame.h"
#include"serverframe.h"

namespace WebSocket {

class WebSocket{

private:

    Socket * socket;
    Epoller epoller;
    Frame * frame;

    int reqFd[2];
    int respFd[2];

    std::string getFileName(const std::string &url);
    void error(const std::string &err);
    bool readClient();
    bool readChild();

public:

    WebSocket(Socket * socket);
    ~WebSocket();

    void handShake(http::Request & request, http::Response & response);

    void process(const std::string &url);

};

};

#endif