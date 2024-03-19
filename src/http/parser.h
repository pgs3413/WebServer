#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include<vector>
#include"request.h"
#include<assert.h>

namespace http {

class Parser {

private:

    enum STATE {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    STATE state ;

    std::vector<char> & buf;
    size_t pos;

    Request & request;

    long searchCRLF();
    long searchCharactor(char c, size_t start, size_t end);
    bool parseRequestLine();
    void parseQueryParameter();

public:

    Parser(std::vector<char> &_buf, Request &_request);
    ~Parser();
    bool parse();
    bool isFinish();

};

};

#endif