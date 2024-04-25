#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include<string>
#include<vector>
#include<cstdlib>
#include"sha1.hpp"
#include"base64.h"

std::vector<std::string> 
split(const std::string &str, char delimiter);

bool isNumber(const std::string &s);

long toNumber(const std::string &s, long defaultValue);

std::string sha1AndBase64(const std::string &s);

void mask(const unsigned char* const mask_key, int mask_key_len, const unsigned char* const payload, unsigned char* const des, int payload_len);


#endif