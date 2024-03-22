#include"connection.h"

namespace http {

Connction::Connction(Socket &&socket_) : 
request(nullptr),
parser(nullptr),
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
    std::unique_ptr<Request> request_temp(new Request());
    request = std::move(request_temp);
    std::unique_ptr<Parser> parser_temp(new Parser(request->buf, *request));
    parser = std::move(parser_temp);
}

/*
是否处理完了请求
TRUE包括请求成功、请求失败（解析失败、内部失败等），此时已正常生成响应
FALSE指的是请求体还未读完，需要继续等待后续读取
*/
bool Connction::processRequest(){
    assert(request != nullptr);
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
    if(!parser->parse()){
        if(parser->isFinish()) {
            return true; //解析错误
        }else {
            return false;
        }
    }
    return true;
}

};