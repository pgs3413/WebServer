#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include<string>
#include<unordered_set>

namespace http {

class Request {

private:
    std::string method;
    std::string url;
    std::string version;

public:

    static const std::unordered_set<std::string> METHOD_SET;
    static const std::unordered_set<std::string> VERSION_SET; 

    Request();
    ~Request();

    const std::string & getMethod() const;
    void setMethod(const std::string &);
    const std::string & getUrl() const;
    void setUrl(const std::string &);
    const std::string & getVersion() const;
    void setVersion(const std::string &);

};


};

#endif