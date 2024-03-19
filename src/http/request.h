#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include<string>
#include<unordered_set>
#include<unordered_map>
#include<vector>

namespace http {

class Request {

private:
    std::string method;
    std::string url;
    std::string version;

    std::unordered_map<std::string, std::string> parametersMap;

public:

    static const std::unordered_set<std::string> METHOD_SET;
    static const std::unordered_set<std::string> VERSION_SET; 

    Request();
    ~Request();

    std::string  getMethod() const;
    void setMethod(const std::string &);

    std::string getUrl() const;
    void setUrl(const std::string &);

    std::string getVersion() const;
    void setVersion(const std::string &);

    std::string getParameter(const std::string &);
    std::vector<std::string> getParameterNames();
    void setParameter(const std::string &, const std::string &);

};


};

#endif