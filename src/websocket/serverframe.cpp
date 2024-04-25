#include"serverframe.h"
#include<string.h>

namespace WebSocket{

ServerFrame::ServerFrame() : buf(nullptr), frameLen(0) {}

ServerFrame::~ServerFrame(){
    if(buf != nullptr){
        delete [] buf;
    }
}

void ServerFrame::setPayload(const unsigned char* const payload, unsigned long len){

    frameLen = 2;
    if(len > 125 && len < 65536) frameLen += 2;
    if(len >= 65536) frameLen += 8;
    frameLen += len;

    buf = new unsigned char[frameLen];

    buf[0] = 0x81;

    if(len <= 125){
        buf[1] = static_cast<unsigned char>(len);
        memcpy(buf + 2, payload, len);
    }else if(len < 65536){
        buf[1] = 126;
        buf[2] = static_cast<unsigned char>(len >> 8);
        buf[3] = static_cast<unsigned char>(len);
        memcpy(buf + 4, payload, len);
    }else {
        buf[1] = 127;
        for(int i = 0; i < 8; i++){
            buf[i + 2] = static_cast<unsigned char>(len >> (8 * (7 - i)));
        }
        memcpy(buf + 10, payload, len);
    }

}

const unsigned char * ServerFrame::data(){
    return buf;
}

unsigned long ServerFrame::size(){
    return frameLen;
}

};