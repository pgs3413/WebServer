#include "response.h"

namespace http {

std::unordered_map<int, std::string> Response::statusMap = {
    {OK, "OK"},
    {BAD_REQUEST, "Bad Request"}, {NOT_FOUND, "Not Found"},
    {SWITCHING_PROTOCOLS, "Switching Protocols"},
};

std::string Response::getStatusStr(int status){
    return statusMap[status];
}

Response::Response() : 
status(OK),
version("1.1"),
_buf(nullptr)
{
}

Response::~Response()
{
}

int Response::getStatus() const {
    return status;
}

void Response::setStatus(STATUS status){
    this -> status = status;
}

std::string Response::getVersion() const {
    return version;
}

void Response::setVersion(const std::string &version){
    this -> version = version;
}

std::string Response::getHeader(const std::string &key){
    return headersMap[key];
}

std::vector<std::string> Response::getHeaderNames(){
    std::vector<std::string> vec;
    for(auto &pair : headersMap){
        vec.push_back(pair.first);
    }
    return vec;
}
    
void Response::setHeader(const std::string &key, const std::string &value){
    headersMap[key] = value;
}

void Response::write(char *buf_, size_t bufSize){
    buf.insert(buf.end(), buf_, buf_ + bufSize);
}

void Response::write(const std::string &s){
    buf.insert(buf.end(), s.c_str(), s.c_str() + s.size());
}

void Response::write(void *buf_, size_t bufSize, BufHandler handler){
    _buf = buf_;
    _bufSize = bufSize;
    _bufHandler = handler;
}

void Response::postResponse(){
    if(_buf != nullptr){
        _bufHandler(_buf, _bufSize);
    }
}

void * Response::getBuf(){
    if(_buf != nullptr) return _buf;
    return (void *)(&buf[0]);
}

size_t Response::getBufSize(){
    if(_buf != nullptr) return _bufSize;
    return buf.size();
}

};