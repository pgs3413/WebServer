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
        SUCCESS,
        WRONG,
    };

    STATE state ;

    std::vector<char> & buf;
    size_t pos;

    Request & request;

    void parseParameter(const std::string &parameters);
    long searchCRLF();
    long searchCharactor(char c, size_t start, size_t end);
    bool parseRequestLine();
    void parseQueryParameter();
    bool parseHeaders();
    bool parseBody();
    void parseContentParameter(long size);

public:

    Parser(std::vector<char> &_buf, Request &_request);
    ~Parser();
    void parse();
    bool isFinish();
    bool isSuccess();
    bool isWrong();

};

};

#endif