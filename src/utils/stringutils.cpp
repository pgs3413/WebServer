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