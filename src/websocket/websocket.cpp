#include "websocket.h"

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

    epoller.addFd(*socket, true, false, true, false, false);

    while (epoller.wait(-1))
    {
        for(auto event : epoller){

        if(event.getFd() == *socket){

            if(event.isClose() || event.isErr()) {
                return;
            }

            if(event.isIn()){

                unsigned char buf[1024];
                int len;
                std::cout << "收到消息: ";

                while ((len = socket->readSocket(buf, 1024)) > 0)
                {
                    for(int i = 0; i < len; i++){
                        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buf[i]);
                    }
                }
                
                std::cout << std::endl;

            }

        }

    }
    }
    

}

