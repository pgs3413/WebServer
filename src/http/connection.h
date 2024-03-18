#ifndef HTTP_CONNECTION_H_
#define HTTP_CONNECTION_H_

#include "../socket/socket.h"
#include "request.h"
#include "parser.h"
#include<vector>
#include<assert.h>
#include<errno.h>

namespace http {

class Connction {

private:

    bool isClose;

    Socket socket;

    std::vector<char> readBuf;
    Request request;
    Parser parser;


public:

    Connction(Socket &&socket);
    ~Connction();
    operator int();

    Request & getRequest();

    bool processRequest();
    void close();

};


};

#endif