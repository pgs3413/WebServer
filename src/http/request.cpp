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
    
void Request::setParameter(const std::string &key, const std::string &value){
    parametersMap[key] = value;
}

};