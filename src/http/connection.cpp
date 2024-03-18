#include"connection.h"

namespace http {

Connction::Connction(Socket &&socket_) : 
isClose(false),
socket(std::move(socket_)),
readBuf(),
request(),
parser(readBuf, request)
{
    socket.setNonBlock();
}

Connction::~Connction(){
    close();
}

Connction::operator int(){
    return socket;
}

Request & Connction::getRequest(){
    return request;
}

/*
是否处理完了请求
TRUE包括请求成功、请求失败（解析失败、内部失败等），此时已正常生成响应
FALSE指的是请求体还未读完，需要继续等待后续读取
*/
bool Connction::processRequest(){
    assert(isClose == false);
    char buf[1024];
    int len = -1;
    long count = 0;
    while ((len = socket.readSocket(buf,1024)) > 0)
    {
        count += len;
        readBuf.insert(readBuf.end(), buf, buf + len);
    }
    if(count == 0){
        return false;
    }
    if(len == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK)){
        return true; // 内部错误
    }
    if(!parser.parse()){
        if(parser.isFinish()) {
            return true; //解析错误
        }else{
            return false;
        }
    }
    return true;
}

void Connction::close(){
    if(!isClose){
        isClose = true;
        socket.closeSocket();
    }
}

};