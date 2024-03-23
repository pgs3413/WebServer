#include"request.h"

namespace http {

const std::unordered_set<std::string> Request::METHOD_SET({
    "GET", 
    "POST",
    });

const std::unordered_set<std::string> Request::VERSION_SET({
    "1.1",
    });

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

};