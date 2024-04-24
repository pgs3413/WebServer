#ifndef HTTP_CONNECTION_H_
#define HTTP_CONNECTION_H_

#include "../socket/socket.h"
#include "request.h"
#include "parser.h"
#include "response.h"
#include "router.h"
#include<vector>
#include<assert.h>
#include<errno.h>
#include<memory>

namespace http {

class Connection {

private:

    enum STATE {
        NOT_INITIAL, REQUEST, RESPONSE, DONE, CLOSED 
    };

    STATE state;
    Socket socket;
    std::unique_ptr<Request> request;
    std::unique_ptr<Parser> parser;
    std::unique_ptr<Response> response;

    static std::string CRLF;

    bool _isWebSocket;

    std::string getResponseHeader();
    void handleWebSocket();

public:

    Connection(Socket &&socket);
    ~Connection();
    operator int();

    void init();
    void close();
    Request & getRequest();
    bool processRequest();
    void processResponse();

    bool isKeepAlive();
    bool isWebSocket();
    
    std::string getAddress();
    unsigned short getPort();

    std::string getUrl();

};


};

#endif