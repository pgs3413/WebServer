#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include"../socket/socket.h"
#include"../http/request.h"
#include"../http/response.h"
#include"../utils/stringutils.h"
#include"../epoll/epoller.h"

class WebSocket{

private:

    Socket * socket;
    Epoller epoller;


public:

    WebSocket(Socket * socket);
    ~WebSocket(){}

    void handShake(http::Request & request, http::Response & response);

    void process(const std::string &url);

};

#endif