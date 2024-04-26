#ifndef WEBSOCKET_SERVER_FRAME_H
#define WEBSOCKET_SERVER_FRAME_H

namespace WebSocket{

class ServerFrame{

private:

    unsigned char *buf;
    unsigned long frameLen;

public:

    ServerFrame();
    ~ServerFrame();

    void setPayload(const unsigned char* const payload, unsigned long len);
    void setClose();
    const unsigned char * data();
    unsigned long size();

};

};

#endif