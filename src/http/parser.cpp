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
    return state == FINISH;
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

bool Parser::parse(){
    assert(state != FINISH);
    if(state == REQUEST_LINE){
        if(!parseRequestLine()) return false;
    }
    return true;
}



bool Parser::parseRequestLine(){

    assert(pos == 0 && state == REQUEST_LINE);

    long tail = searchCRLF(); 
    if(tail == -1) return false;

    long methodTail = searchCharactor(' ', pos, tail);
    if(methodTail == -1){
        state = FINISH;
        return false;
    }

    std::string method(buf.begin() + pos, buf.begin() + methodTail);
    if(Request::METHOD_SET.count(method) == 0){
        state = FINISH;
        return false;
    }
    request.setMethod(method);

    long urlTail = searchCharactor(' ', methodTail + 1, tail);
    if(urlTail == -1) {
        state = FINISH;
        return false;
    }

    std::string url(buf.begin() + methodTail + 1, buf.begin() + urlTail);
    if(url.empty() || url[0] != '/'){
        state = FINISH;
        return false;
    }
    request.setUrl(url);

    long versionTail = searchCharactor('\r', urlTail + 1, tail);
    if(versionTail == -1) {
        state = FINISH;
        return false;
    }

    std::string version(buf.begin() + urlTail + 6, buf.begin() + versionTail);
    if(Request::VERSION_SET.count(version) == 0){
        state = FINISH;
        return false;
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

};