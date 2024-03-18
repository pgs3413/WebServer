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

const std::string & Request::getMethod() const {
    return method;
}

void Request::setMethod(const std::string &method){
    this -> method = method;
}

const std::string & Request::getUrl() const {
    return url;
}

void Request::setUrl(const std::string &url) {
    this -> url = url;
}

const std::string & Request::getVersion() const {
    return version;
}

void Request::setVersion(const std::string &version){
    this -> version = version;
}



};