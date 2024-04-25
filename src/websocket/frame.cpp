#include"frame.h"
#include"../utils/stringutils.h"
#include<iostream>

namespace WebSocket{

Frame::Frame() : state(OP_CODE), _isFin(false), op(OTHER), _isMask(false), payloadLen(0), payload(nullptr)
{}

Frame::~Frame(){
    if(payload != nullptr){
        delete [] payload;
    }
}

unsigned long Frame::next(){

    unsigned long nextByteLen = 0;

    switch (state){
    case OP_CODE:
    case PAYLOAD_LEN:
        nextByteLen = 1;
        break;
    case EXTENDED_PAYLOAD_LEN:
        if(payloadLen == 126) nextByteLen = 2;
        if(payloadLen == 127) nextByteLen = 8;
        break;
    case MASK_KEY:
        nextByteLen = 4;
        break;
    case PAYLOAD_DATA:
        nextByteLen = payloadLen;
        break;
    default:
        break;
    }

    return nextByteLen;

}

void Frame::parse(const unsigned char* const buf, unsigned long len){

    if(state == OP_CODE){
        parseOpCode(buf, len);
    }

    else if(state == PAYLOAD_LEN){
        parsePayLoadLen(buf, len);
    }

    else if(state == EXTENDED_PAYLOAD_LEN){
        parseExtendedPayLoadLen(buf, len);
    }

    else if(state == MASK_KEY){
        parseMaskKey(buf, len);
    }

    else if(state == PAYLOAD_DATA){
        parsePayLoadData(buf, len);
    }

}

void Frame::parseOpCode(const unsigned char* const buf, unsigned long len){
    assert(state == OP_CODE && len == 1);
    unsigned char byte = buf[0];
    _isFin = byte & 0x80;
    unsigned char op_code = byte & 0x0F;
    switch (op_code)
    {
    case 0x00:
        op = CONTINUATION;
        break;
    case 0x01:
        op = TEXT;
        break;
    case 0x02:
        op = BINARY;
        break;
    case 0x08:
        op = CONNECTION;
        break;
    case 0x09:
        op = PING;
        break;
    case 0x0A:
        op = PONG;
        break;
    default:
        op = OTHER;
        break;
    }
    state = PAYLOAD_LEN;
}

void Frame::parsePayLoadLen(const unsigned char* const buf, unsigned long len){
    assert(state == PAYLOAD_LEN && len == 1);
    unsigned char byte = buf[0];
    _isMask = byte & 0x80;
    payloadLen = byte & 0x7F;
    assert(payloadLen <= 127);
    if(payloadLen > 125) state = EXTENDED_PAYLOAD_LEN;
    else if(_isMask) state = MASK_KEY;
    else if(payloadLen > 0) state = PAYLOAD_DATA;
    else state = DONE;
}

void Frame::parseExtendedPayLoadLen(const unsigned char* const buf, unsigned long len){
    assert(state == EXTENDED_PAYLOAD_LEN);
    assert(payloadLen == 126 && len == 2 || payloadLen == 127 && len == 8);
    if(payloadLen == 126){
        payloadLen = (static_cast<unsigned long>(buf[0]) << 8)| static_cast<unsigned long>(buf[1]);
    }else{
        unsigned long result = 0;
        for(int i = 0; i < 8; i++){
            result |= static_cast<unsigned long>(buf[i]) << (8 * (7 - i));
        }
        payloadLen = result;
    }
    if(_isMask) state = MASK_KEY;
    else state = PAYLOAD_DATA;
}

void Frame::parseMaskKey(const unsigned char* const buf, unsigned long len){
    assert(state == MASK_KEY);
    assert(len == 4);
    for(int i = 0; i < 4; i++) maskKey[i] = buf[i];
    if(payloadLen > 0) state = PAYLOAD_DATA;
    else state = DONE;
}

void Frame::parsePayLoadData(const unsigned char* const buf, unsigned long len){
    assert(state == PAYLOAD_DATA);
    assert(len > 0 && len == payloadLen);
    payload = new unsigned char[payloadLen];
    mask(maskKey, 4, buf, payload, payloadLen);
    state = DONE;
}

bool Frame::isFin(){
    return _isFin;
}
std::string Frame::getOp(){
    switch (op)
    {
    case CONTINUATION: return "CONTINUATION";
    case TEXT: return "TEXT";
    case BINARY: return "BINARY";
    case CONNECTION: return "CONNECTION";
    case PING: return "PING";
    case PONG: return "PONG";
    default: return "OTHER";
    }
}

bool Frame::isMask(){
    return _isMask;
}

unsigned long Frame::size(){
    return payloadLen;
}

std::string Frame::getMaskKey(){
    if(!_isMask) return "";
    std::ostringstream osstream;
    for(int i = 0; i < 4; i++) {
        osstream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(maskKey[i]);
    }
    return osstream.str();
}

std::string Frame::getpayLoadString(){
    if(payloadLen == 0) return "";
    std::string s;
    for(int i = 0; i < payloadLen; i++){
        s += payload[i];
    }
    return s;
}
bool Frame::isCloseFrame(){
    return op == CONNECTION;
}

};