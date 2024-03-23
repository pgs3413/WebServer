#include"connection.h"

namespace http {

std::string Connction::CRLF = "\r\n";

Connction::Connction(Socket &&socket_) : 
request(nullptr),
parser(nullptr),
response(nullptr),
state(NOT_INITIAL),
socket(std::move(socket_))
{
    socket.setNonBlock();
}

Connction::~Connction(){
    socket.closeSocket();
}

Connction::operator int(){
    return socket;
}

Request & Connction::getRequest(){
    return *request;
}

void Connction::init(){
    state = REQUEST;
    std::unique_ptr<Request> request_temp(new Request());
    request = std::move(request_temp);
    std::unique_ptr<Parser> parser_temp(new Parser(request->buf, *request));
    parser = std::move(parser_temp);
    std::unique_ptr<Response> response_temp(new Response());
    response = std::move(response_temp);
}

/*
是否处理完了请求
TRUE包括请求成功、请求失败（解析失败、内部失败等），此时已正常生成响应
FALSE指的是请求体还未读完，需要继续等待后续读取
*/
bool Connction::processRequest(){
    assert(state == REQUEST);
    assert(request != nullptr && parser != nullptr && response != nullptr);
    char buf[1024];
    int len = -1;
    long count = 0;
    while ((len = socket.readSocket(buf,1024)) > 0)
    {
        count += len;
        request->buf.insert(request->buf.end(), buf, buf + len);
    }
    if(count == 0){
        return false;
    }
    if(len == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK)){
        return true; // 内部错误
    }
    parser -> parse();
    if(parser -> isFinish()){
        if(parser -> isSuccess()){
            Router::getDefaultHandler()(*request, *response);
        }else {
            Router::getErrHandler()(*request, *response);
        }
        state = RESPONSE;
        return true;
    }
    return false;
}

std::string Connction::getResponseHeader(){
    std::string header;
    header += "HTTP/";
    header += response->getVersion();
    header += " ";
    header += std::to_string(response->getStatus());
    header += Response::getStatusStr(response->getStatus());
    header += CRLF;
    for(auto &key : response->getHeaderNames()){
        header += key;
        header += ": ";
        header += response->getHeader(key);
        header += CRLF;
    }
    header += CRLF;
    return header;
}

void Connction::processResponse(){
    assert(state == RESPONSE);
    assert(response != nullptr);

    response -> setHeader("Content-Length", std::to_string(response->buf.size()));
    
    std::string header = getResponseHeader();

    struct iovec vecs[2] = {
        {.iov_base = (void *)header.c_str(), .iov_len = header.size()},
        {.iov_base = (void *)(&(response -> buf[0])), .iov_len = response -> buf.size()}
    };

    socket.writevSocket(vecs, 2);
    state = DONE;
}

bool Connction::isKeepAlive(){
    assert(request != nullptr);
    return request -> getHeader(Request::CONNECTION_KEY) == "keep-alive";
}

};