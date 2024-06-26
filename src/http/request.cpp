#include"request.h"

namespace http {

const std::unordered_set<std::string> Request::METHOD_SET = {
    "GET", 
    "POST",
    };

const std::unordered_set<std::string> Request::VERSION_SET = {
    "1.1",
    };

const std::string Request::CONTENT_LENGTH_KEY = "Content-Length";
const std::string Request::CONTENT_TYPE_KEY = "Content-Type";
const std::string Request::CONNECTION_KEY = "Connection";


Request::Request(){
}

Request::~Request(){    
}

std::string Request::getMethod() const {
    return method;
}

void Request::setMethod(const std::string &method){
    this -> method = method;
}

std::string Request::getUrl() const {
    return url;
}

void Request::setUrl(const std::string &url) {
    this -> url = url;
}

std::string Request::getVersion() const {
    return version;
}

void Request::setVersion(const std::string &version){
    this -> version = version;
}

std::string Request::getParameter(const std::string &key){
    if(parametersMap.count(key) == 0) return "";
    return parametersMap[key];
}

std::vector<std::string> Request::getParameterNames(){
    std::vector<std::string> vec;
    for(auto &pair : parametersMap){
        vec.push_back(pair.first);
    }
    return vec;
}
    
void Request::setParameter(const std::string &key, const std::string &value){
    parametersMap[key] = value;
}

std::string Request::getHeader(const std::string &key){
    if(headersMap.count(key) == 0) return "";
    return headersMap[key];
}

std::vector<std::string> Request::getHeaderNames(){
    std::vector<std::string> vec;
    for(auto &pair : headersMap){
        vec.push_back(pair.first);
    }
    return vec;
}
    
void Request::setHeader(const std::string &key, const std::string &value){
    headersMap[key] = value;
}

size_t Request::getBody(char *buf_ ,size_t bufSize){
    if(bodySize == 0){
        return 0;
    }
    size_t count = bodySize > bufSize ? bufSize : bodySize;
    std::memcpy(buf_, &(buf[bodyStart]), count);
    bodyStart += count;
    bodySize -= count;
    return count;
}

std::string Request::getDefaultPage(){
    return "/index.html";
}

};