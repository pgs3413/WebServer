#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include<string>
#include<unordered_map>
#include<vector>
#include<functional>

namespace http {

class Response {

public:

    enum STATUS {
        OK = 200,
        BAD_REQUEST = 400, NOT_FOUND = 404,
        SWITCHING_PROTOCOLS = 101
   };

private:

    std::string version;
    STATUS status;

    static std::unordered_map<int, std::string> statusMap;

    std::unordered_map<std::string, std::string> headersMap;

    std::vector<char> buf;

    typedef std::function<void(void*,size_t)> BufHandler;
    void *_buf;
    size_t _bufSize;
    BufHandler _bufHandler;

    void postResponse();
    void * getBuf();
    size_t getBufSize();

public:

    friend class Connection;

    Response();
    ~Response();

    static std::string getStatusStr(int status);

    int getStatus() const;
    void setStatus(STATUS status);

    std::string getVersion() const;
    void setVersion(const std::string &);

    std::string getHeader(const std::string &);
    std::vector<std::string> getHeaderNames();
    void setHeader(const std::string &, const std::string &);

    void write(char *buf_, size_t bufSize);
    void write(const std::string &s);
    void write(void *buf_, size_t bufSize, BufHandler handler);

};

};

#endif