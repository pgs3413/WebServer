#include "base64.h"

// 编解码转换表
unsigned char CBASE64::s_encTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
unsigned char CBASE64::s_decTable[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3E,0xFF,0xFF,0xFF,0x3F,
    0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
    0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
    0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF
};

// 执行BASE64编码操作
std::string CBASE64::encode(const std::string& str)
{
    std::string strEncode;
    strEncode.resize( (str.size() / 3 + 1) * 4 );
    strEncode.resize( __encode((unsigned char*)strEncode.data(), (const unsigned char*)str.data(), str.size()) );
    return strEncode;
}

// 执行BASE64解码操作
std::string CBASE64::decode(const std::string& str)
{
    std::string strDecode;
    strDecode.resize( str.size() );
    strDecode.resize( __decode((unsigned char*)strDecode.data(), (const unsigned char*)str.data(), str.size()) );
    return strDecode;
}

// 分组编码
int CBASE64::__encode(unsigned char* pDest, const unsigned char* pSrc, size_t nSrcLen)
{
    unsigned char * dst = pDest;
    const unsigned char * src = pSrc;
    size_t len = nSrcLen;

    unsigned char * odst = dst;
    unsigned long l = 0, m = 0, n = 0;

    // 循环处理分组
    size_t off = 0;
    for ( ; off + 3 <= len; off += 3)
    {
        l = *src++;
        m = *src++;
        n = *src++;

        *dst++ = s_encTable[ (l >> 2) & 0x3F ];
        *dst++ = s_encTable[ ((l << 4) & 0x30) | ((m >> 4) & 0x0F) ];
        *dst++ = s_encTable[ ((m << 2) & 0x3C) | ((n >> 6) & 0x03) ];
        *dst++ = s_encTable[ n & 0x3F ];
    }

    // 处理余下的2个字节
    if (off + 2 <= len)
    {
        l = *src++;
        m = *src++;

        *dst++ = s_encTable[ (l >> 2) & 0x3F ];
        *dst++ = s_encTable[ ((l << 4) & 0x30) | ((m >> 4) & 0x0F) ];
        *dst++ = s_encTable[ ((m << 2) & 0x3C) ];
        *dst++ = '=';
    }

    // 处理余下的1个字节
    else if (off + 1 <= len)
    {
        l = *src++;

        *dst++ = s_encTable[ (l >> 2) & 0x3F ];
        *dst++ = s_encTable[ ((l << 4) & 0x30) ];
        *dst++ = '=';
        *dst++ = '=';
    }

    return (int)(dst - odst);
}

// 分组解码
int CBASE64::__decode(unsigned char* pDest, const unsigned char* pSrc, size_t nSrcLen)
{
    unsigned char * dst = pDest;
    const unsigned char * src = pSrc;
    size_t len = nSrcLen;

    unsigned char * odst = dst;
    unsigned long l = 0, m = 0, n = 0, o = 0;

    // 循环处理分组
    size_t off = 0;
    for (; off + 4 <= len; off += 4)
    {
        if ( (src[0] > 0x7F) || (src[1] > 0x7F) || (src[2] > 0x7F) || (src[3] > 0x7F) ) return (int)(dst - odst);
        if ( (src[0] == '=') || (src[1] == '=') || (src[2] == '=') || (src[3] == '=') ) break;

        l = s_decTable[*src++];
        m = s_decTable[*src++];
        n = s_decTable[*src++];
        o = s_decTable[*src++];

        *dst++ = (unsigned char)( ((l << 2) & 0xFC) | ((m >> 4) & 0x03) );
        *dst++ = (unsigned char)( ((m << 4) & 0xF0) | ((n >> 2) & 0x0F) );
        *dst++ = (unsigned char)( ((n << 6) & 0xC0) | (o & 0x3F));
    }

    // 处理余下的3个字节
    if (off + 3 <= len)
    {
        if ( (src[0] != '=') && (src[1] != '=') )
        {
            l = s_decTable[*src++];
            m = s_decTable[*src++];

            *dst++ = (unsigned char)( ((l << 2) & 0xFC) | ((m >> 4) & 0x03) );
        }

        if ( (src[2] != '=') )
        {
            n = s_decTable[*src++];

            *dst++ = (unsigned char)( ((m << 4) & 0xF0) | ((n >> 2) & 0x0F) );
        }
    }

    // 处理余下的两个字节
    else if (off + 2 <= len)
    {
        if ( (src[0] != '=') && (src[1] != '=') )
        {
            l = s_decTable[*src++];
            m = s_decTable[*src++];

            *dst++ = (unsigned char)( ((l << 2) & 0xFC) | ((m >> 4) & 0x03) );
        }
    }

    return (int)(dst - odst);
}