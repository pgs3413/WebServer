#include "websocket.h"

namespace WebSocket{

WebSocket::WebSocket(Socket * socket) : socket(socket) {}

void WebSocket::handShake(http::Request & request, http::Response & response){
    response.setStatus(http::Response::SWITCHING_PROTOCOLS);
    response.setHeader("Connection","Upgrade");
    response.setHeader("Upgrade","websocket");
    response.setHeader("Sec-WebSocket-Version",request.getHeader("Sec-WebSocket-Version"));
    std::string s = request.getHeader("Sec-WebSocket-Key");
    s += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    response.setHeader("Sec-WebSocket-Accept",sha1AndBase64(s));
}

void WebSocket::process(const std::string &url){

    epoller.addFd(*socket, true, false, true, true, false);

    Frame *frame = new Frame;

    while (epoller.wait(-1))
    {
        for(auto event : epoller){

        if(event.getFd() == *socket){

            if(event.isClose() || event.isErr()) {
                delete frame;
                return;
            }

            if(event.isIn()){

                while (true)
                {

                unsigned long needLen = frame->next();
                if(needLen == 0){
                    std::cout << "isFin: " << frame->isFin()
                    << " isMask: " << frame->isMask()
                    << " op: " << frame->getOp()
                    << " mask key: " << frame->getMaskKey()
                    << " payload size: " << frame->size()
                    << " payload: " << frame->getpayLoadString()
                    << std::endl;
                    bool isClose = frame->isCloseFrame();
                    delete frame;
                    if(isClose) return;
                    frame = new Frame;
                    continue;
                }

                int availableLen = socket->readableBytes();
                if(needLen > availableLen) break;
                
                unsigned char buf[needLen];
                int len = socket->readSocket(buf,needLen);
                assert(len == needLen);
                frame->parse(buf, needLen);

                }
                

            }

        }

        }
    }
    

}

};

