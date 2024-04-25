#include"connection.h"

namespace http {

std::string Connection::CRLF = "\r\n";

Connection::Connection(Socket &&socket_) : 
request(nullptr),
parser(nullptr),
response(nullptr),
webSocket(nullptr),
state(NOT_INITIAL),
_isWebSocket(false),
socket(std::move(socket_))
{
    socket.setNonBlock();
}

Connection::~Connection(){
    socket.closeSocket();
}

Connection::operator int(){
    return socket;
}

Request & Connection::getRequest(){
    return *request;
}

void Connection::init(){
    assert(state != CLOSED);
    state = REQUEST;
    std::unique_ptr<Request> request_temp(new Request());
    request = std::move(request_temp);
    std::unique_ptr<Parser> parser_temp(new Parser(request->buf, *request));
    parser = std::move(parser_temp);
    std::unique_ptr<Response> response_temp(new Response());
    response = std::move(response_temp);
}

void Connection::close(){
    socket.closeSocket();
    state = CLOSED;
}

/*
是否处理完了请求
TRUE包括请求成功、请求失败（解析失败、内部失败等），此时已正常生成响应
FALSE指的是请求体还未读完，需要继续等待后续读取
*/
bool Connection::processRequest(){
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
            if(request->getHeader("Connection") == "Upgrade" 
                && request->getHeader("Upgrade") == "websocket"){
                    _isWebSocket = true;
                    std::unique_ptr<WebSocket::WebSocket> webSocket_temp(new WebSocket::WebSocket(&socket));
                    webSocket = std::move(webSocket_temp);
                    webSocket->handShake(*request,*response);
                }
            else {
                Router::route(*request, *response);
            }
        }else {
            Router::getErrHandler()(*request, *response);
        }
        state = RESPONSE;
        return true;
    }
    return false;
}

std::string Connection::getResponseHeader(){
    std::string header;
    header += "HTTP/";
    header += response->getVersion();
    header += " ";
    header += std::to_string(response->getStatus());
    header += " ";
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

void Connection::processResponse(){
    assert(state == RESPONSE);
    assert(response != nullptr);

    response -> setHeader("Content-Length", std::to_string(response -> getBufSize()));
    
    std::string header = getResponseHeader();

    struct iovec vecs[2] = {
        {.iov_base = (void *)header.c_str(), .iov_len = header.size()},
        {.iov_base = response -> getBuf(), .iov_len = response -> getBufSize()}
    };

    socket.writevSocket(vecs, 2);
    response -> postResponse();
    state = DONE;

    if(isWebSocket()) webSocket->process(request->getUrl());
}

bool Connection::isKeepAlive(){
    assert(request != nullptr);
    return request -> getHeader(Request::CONNECTION_KEY) == "keep-alive";
}

bool Connection::isWebSocket(){
    return _isWebSocket;
}

std::string Connection::getAddress(){
    return socket.getAddress();
}

unsigned short Connection::getPort(){
    return socket.getPort();
}

std::string Connection::getUrl(){
    return request -> getUrl();
}

};