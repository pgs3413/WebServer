#ifndef HTTP_CONNECTION_H_
#define HTTP_CONNECTION_H_

#include "../socket/socket.h"
#include "request.h"
#include "parser.h"
#include<vector>
#include<assert.h>
#include<errno.h>
#include<memory>

namespace http {

class Connction {

private:

    Socket socket;
    std::unique_ptr<Request> request;
    std::unique_ptr<Parser> parser;

public:

    Connction(Socket &&socket);
    ~Connction();
    operator int();

    void init();
    Request & getRequest();
    bool processRequest();

};


};

#endif