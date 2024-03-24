#include "parser.h"
#include "../utils/stringutils.h"

namespace http {

Parser::Parser(std::vector<char> &_buf, Request &_request)
: state(REQUEST_LINE), buf(_buf), request(_request), pos(0) 
{
}

Parser::~Parser()
{
}

bool Parser::isFinish(){
    return state == SUCCESS || state == WRONG;
}

bool Parser::isSuccess(){
    return state == SUCCESS;
}

bool Parser::isWrong(){
    return state == WRONG;
}

void Parser::parseParameter(const std::string &parameters){
    auto kvList = split(parameters, '&');
    for(auto &kv : kvList){
        if(kv.empty()) continue;;
        auto equalIndex = kv.find('=');
        if(equalIndex == std::string::npos){
            request.setParameter(kv, "");
        }else{
            request.setParameter(kv.substr(0, equalIndex), 
            kv.substr(equalIndex + 1, kv.size() - equalIndex - 1));
        }
    }
}

long Parser::searchCRLF(){
    long result = -1;
    size_t start = pos;
    size_t size = buf.size();
    while (start < size)
    {
        if(buf[start] == '\r' && start + 1 < size && buf[start + 1] == '\n'){
            result = start + 1;
            break;
        }else{
            start++;
        }
    }
    return result;
}

long Parser::searchCharactor(char c, size_t start, size_t end){
    long result = -1;
    while (start <= end)
    {
        if(buf[start] == c){
            result = start;
            break;
        }else{
            start++;
        }
    }
    return result;
}

void Parser::parse(){
    if(state == REQUEST_LINE){
        if(!parseRequestLine()) return;
    }
    while(state == HEADERS){
        if(!parseHeaders()) return;
    }
    if(state == BODY){
        if(!parseBody()) return;
    }
}

/*
 所有的parse true代表可以往下继续走,false代表需要下次执行
*/

bool Parser::parseRequestLine(){

    assert(pos == 0 && state == REQUEST_LINE);

    long tail = searchCRLF(); 
    if(tail == -1) return false;

    long methodTail = searchCharactor(' ', pos, tail);
    if(methodTail == -1){
        state = WRONG;
        return true;
    }

    std::string method(buf.begin() + pos, buf.begin() + methodTail);
    if(Request::METHOD_SET.count(method) == 0){
        state = WRONG;
        return true;
    }
    request.setMethod(method);

    long urlTail = searchCharactor(' ', methodTail + 1, tail);
    if(urlTail == -1) {
        state = WRONG;
        return true;
    }

    std::string url(buf.begin() + methodTail + 1, buf.begin() + urlTail);
    if(url.empty() || url[0] != '/'){
        state = WRONG;
        return true;
    }

    if(url == "/") url = request.getDefaultPage();
    request.setUrl(url);

    long versionTail = searchCharactor('\r', urlTail + 1, tail);
    if(versionTail == -1) {
        state = WRONG;
        return true;
    }

    std::string version(buf.begin() + urlTail + 6, buf.begin() + versionTail);
    if(Request::VERSION_SET.count(version) == 0){
        state = WRONG;
        return true;
    }
    request.setVersion(version);

    parseQueryParameter();
    pos = tail + 1;
    state = HEADERS;
    return true;
}

void Parser::parseQueryParameter(){
    std::string url = request.getUrl();
    auto start = url.find('?');
    if(start == std::string::npos){
        return;
    }
    request.setUrl(url.substr(0, start));
    std::string parameters = url.substr(start + 1, url.size() - start - 1);
    parseParameter(parameters);
}

bool Parser::parseHeaders(){
    assert(state == HEADERS);

    long tail = searchCRLF(); 
    if(tail == -1) return false;

    if(tail == pos + 1){ //最后的/r/n
        pos = tail + 1;
        state = BODY;
        return true;
    }

    long colonIndex = searchCharactor(':', pos, tail);
    if(colonIndex == -1){
        state = WRONG;
        return true;
    }

    std::string key(buf.begin() + pos, buf.begin() + colonIndex);
    if(key.empty()){
        state = WRONG;
        return true;
    }

    std::string value(buf.begin() + colonIndex + 2, buf.begin() + tail - 1);

    request.setHeader(key, value);
    pos = tail + 1;
    return true;
}

bool Parser::parseBody(){
    assert(state == BODY);

    long size = toNumber(request.getHeader(Request::CONTENT_LENGTH_KEY), 0);
    if(size == 0){
        state = SUCCESS;
        request.bodySize = 0;
        return true;
    }

    if(buf.size() < pos + size){
        return false;
    }

    request.bodyStart = pos;
    request.bodySize = size;

    parseContentParameter(size);

    state = SUCCESS;
    return true;

}

void Parser::parseContentParameter(long size){
    if(request.getHeader(Request::CONTENT_TYPE_KEY) 
    != "application/x-www-form-urlencoded"){
        return;
    }
    std::string parameters(buf.begin() + pos, buf.begin() + pos + size);
    auto kvList = split(parameters, '&');
    parseParameter(parameters);
}

};