#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include<string>
#include<vector>
#include<cstdlib>

std::vector<std::string> 
split(const std::string &str, char delimiter);

bool isNumber(const std::string &s);

long toNumber(const std::string &s, long defaultValue);

std::string sha1AndBase64(const std::string &s);


#endif