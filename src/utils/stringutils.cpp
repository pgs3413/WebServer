#include "stringutils.h"

std::vector<std::string> 
split(const std::string &str, char delimiter){

    std::vector<std::string> vec;

    size_t start = 0;
    size_t size = str.size();

    for(int i = 0; i < size; i++){
        if(str[i] == delimiter){
            vec.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    vec.push_back(str.substr(start, size - start));
    return vec;
}

bool isNumber(const std::string &s){
    char *endPtr;
    long result = std::strtol(s.c_str(), &endPtr, 0);
    if(endPtr == s.c_str() || *endPtr != '\0'){
        return false;
    }
    return true;
}

long toNumber(const std::string &s, long defaultValue){
    char *endPtr;
    long result = std::strtol(s.c_str(), &endPtr, 0);
    if(endPtr == s.c_str() || *endPtr != '\0'){
        return defaultValue;
    }
    return result;
}