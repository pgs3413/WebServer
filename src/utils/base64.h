#ifndef __DAKUANG_BASE64_H__
#define __DAKUANG_BASE64_H__

#include <string>

    class CBASE64
    {
    public:

        // 执行BASE64编码操作
        static std::string encode(const std::string& str);

        // 执行BASE64解码操作
        static std::string decode(const std::string& str);

    private:

        // 分组编码
        static int __encode(unsigned char* pDest, const unsigned char* pSrc, size_t nSrcLen);

        // 分组解码
        static int __decode(unsigned char* pDest, const unsigned char* pSrc, size_t nSrcLen);

    private:

        // 编解码转换表
        static unsigned char s_encTable[];
        static unsigned char s_decTable[];
    };


#endif