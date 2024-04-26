#include "websocket.h"
#include<signal.h>

namespace WebSocket{

WebSocket::WebSocket(Socket * socket) : socket(socket) {
    reqFd[0] = -1;
    reqFd[1] = -1;
    respFd[0] = -1;
    respFd[1] = -1;
    frame = new Frame;
}

WebSocket::~WebSocket(){
    close(reqFd[0]);
    close(reqFd[1]);
    close(respFd[0]);
    close(respFd[1]);
    delete frame;
}

void WebSocket::handShake(http::Request & request, http::Response & response){
    response.setStatus(http::Response::SWITCHING_PROTOCOLS);
    response.setHeader("Connection","Upgrade");
    response.setHeader("Upgrade","websocket");
    response.setHeader("Sec-WebSocket-Version",request.getHeader("Sec-WebSocket-Version"));
    std::string s = request.getHeader("Sec-WebSocket-Key");
    s += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    response.setHeader("Sec-WebSocket-Accept",sha1AndBase64(s));
}

std::string WebSocket::getFileName(const std::string &url){

    char *ws_path = std::getenv("ws_path");
    std::string wsPath = ws_path == nullptr ? "./ws/" : ws_path;

    char *shell_prefix = std::getenv("shell_prefix");
    std::string shellPrefix = shell_prefix == nullptr ? "/sh/" : shell_prefix;

    bool isShell = false;

    if(url.compare(0, shellPrefix.size(), shellPrefix) == 0){
        isShell = true;
    }

    std::string fileName;

    if(isShell){
        fileName = url.substr(shellPrefix.size());
    }else {
        if(url[0] == '/') fileName = wsPath + url.substr(1);
        else fileName = wsPath + url;
    }

    return fileName;

}

void WebSocket::error(const std::string &err){
    ServerFrame frame;
    frame.setPayload((const unsigned char *)(err.c_str()), err.size());
    socket->writeSocket(frame.data(), frame.size());
    frame.setClose();
    socket->writeSocket(frame.data(), frame.size());
}

void WebSocket::process(const std::string &url){

    std::string fileName = getFileName(url);

    if(pipe(reqFd) == -1 || pipe(respFd) == -1){
        error("could not create pipe");
        close(reqFd[0]);
        close(reqFd[1]);
        close(respFd[0]);
        close(respFd[1]);
        return;
    }

    pid_t pid = fork();

    if(pid == -1){
        error("could not fork");
        close(reqFd[0]);
        close(reqFd[1]);
        close(respFd[0]);
        close(respFd[1]);
        return;
    }  

    if(pid == 0){

        struct rlimit rlim;
        getrlimit(RLIMIT_NOFILE, &rlim);

        dup2(reqFd[0], 0);
        // dup2(respFd[1], 1);
        dup2(respFd[1], 2);

        for(int i = 3; i <= rlim.rlim_cur; i++){
            close(i);
        }

        execlp(fileName.c_str(), fileName.c_str(), nullptr);

        write(2, "something wrong: ", 17);
        perror(nullptr);

        exit(1);

    } else {

        close(reqFd[0]);
        close(respFd[1]);

        int flags = fcntl(respFd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(respFd[0], F_SETFL, flags);

        epoller.addFd(*socket, true, false, true, true, false);
        epoller.addFd(respFd[0], true, false, true, false, false);

        while (epoller.wait(-1))
        {
            for(auto event : epoller){

                if(event.getFd() == *socket){

                    if(event.isClose() || event.isErr()) {
                        kill(pid, SIGINT);
                        waitpid(pid, nullptr, 0);
                        return;
                    }

                    if(event.isIn()){
                        bool isClose = readClient();
                        if(isClose){
                            kill(pid, SIGINT);
                            waitpid(pid, nullptr, 0);
                            return;
                        }
                    }

                }

                if(event.getFd() == respFd[0]){

                    // if(event.isClose() || event.isErr()) {
                    //     assert(1 == 0);
                    // }

                    if(event.isIn()){

                        bool isClose = readChild();
                        if(isClose){
                            error("server closed");
                            waitpid(pid, nullptr, 0);
                            return;
                        }

                    }

                }

            }
        }
        

    }
    

}

bool WebSocket::readClient(){

    while (true){

        unsigned long needLen = frame->next();
        if(needLen == 0){
            bool isClose = frame->isCloseFrame();
            if(isClose) return true;
            write(reqFd[1], frame->getPayLoad(), frame->size());
            delete frame;
            frame = new Frame;
            continue;
        }
        
        int availableLen = socket->readableBytes();
        if(needLen > availableLen) return false;
                
        unsigned char buf[needLen];
        int len = socket->readSocket(buf,needLen);
        assert(len == needLen);
        frame->parse(buf, needLen);
       
    }

}

bool WebSocket::readChild(){
    int len;
    unsigned char childBuf[1024];
    while ((len = read(respFd[0], childBuf, 1024)) > 0)
    {
        ServerFrame serverFrame;
        serverFrame.setPayload(childBuf, len);
        socket->writeSocket(serverFrame.data(), serverFrame.size());
    }
    if(len == 0) return true;
    else return false;
}

};

