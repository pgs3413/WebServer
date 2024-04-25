#ifndef WEBSOCKET_FRAME_H
#define WEBSOCKET_FRAME_H

#include<assert.h>
#include<string>

namespace WebSocket{

class Frame {

private:

    enum STATE {OP_CODE, PAYLOAD_LEN, EXTENDED_PAYLOAD_LEN, MASK_KEY, PAYLOAD_DATA, DONE};
    enum OP{CONTINUATION, TEXT, BINARY, CONNECTION, PING, PONG, OTHER};

    STATE state;
    bool _isFin;
    OP op;
    bool _isMask;
    unsigned long payloadLen;
    unsigned char maskKey[4];
    unsigned char * payload;

    void parseOpCode(const unsigned char* const buf, unsigned long len);
    void parsePayLoadLen(const unsigned char* const buf, unsigned long len);
    void parseExtendedPayLoadLen(const unsigned char* const buf, unsigned long len);
    void parseMaskKey(const unsigned char* const buf, unsigned long len);
    void parsePayLoadData(const unsigned char* const buf, unsigned long len);



public:

    Frame();
    ~Frame();

    unsigned long next();
    void parse(const unsigned char* const buf, unsigned long len);

    bool isFin();
    std::string getOp();
    bool isMask();
    bool isCloseFrame();
    unsigned long size();
    std::string getMaskKey();
    std::string getpayLoadString();

};


};

#endif