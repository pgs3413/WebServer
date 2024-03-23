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

class Connction {

private:

    enum STATE {
        NOT_INITIAL, REQUEST, RESPONSE, DONE 
    };

    STATE state;
    Socket socket;
    std::unique_ptr<Request> request;
    std::unique_ptr<Parser> parser;
    std::unique_ptr<Response> response;

    static std::string CRLF;

    std::string getResponseHeader();

public:

    Connction(Socket &&socket);
    ~Connction();
    operator int();

    void init();
    Request & getRequest();
    bool processRequest();
    void processResponse();

    bool isKeepAlive();

};


};

#endif