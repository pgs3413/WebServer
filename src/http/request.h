#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include<string>
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<cstring>

namespace http {

class Request {

private:

    std::vector<char> buf;

    std::string method;
    std::string url;
    std::string version;

    std::unordered_map<std::string, std::string> parametersMap;
    std::unordered_map<std::string, std::string> headersMap;

    size_t bodyStart = 0;
    size_t bodySize = 0;

public:

    friend class Connction;
    friend class Parser;

    static const std::unordered_set<std::string> METHOD_SET;
    static const std::unordered_set<std::string> VERSION_SET;

    static const std::string CONTENT_LENGTH_KEY;
    static const std::string CONTENT_TYPE_KEY;
    static const std::string CONNECTION_KEY;

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

    std::string getHeader(const std::string &);
    std::vector<std::string> getHeaderNames();
    void setHeader(const std::string &, const std::string &);

    size_t getBody(char *buf_, size_t bufSize);

};


};

#endif