#include"../src/http/connection.h"
#include"../src/socket/socket.h"
#include"../src/epoll/epoller.h"
#include<iostream>

using std::cout;
using std::endl;

int main(){

    ServerSocket server(8080);
    cout << "start to listen..." << endl;
    Socket socket = server.acceptSocket();
    cout << "listened." << endl;

    http::Connction conn(std::move(socket));

    Epoller epoller;
    epoller.addFd(conn, true, false, true, true, false);

    cout << "start to read..." << endl;

    while (epoller.wait())
    {
        for(auto event : epoller){
            if(event.getFd() == conn){
                if(event.isIn()){
                    if(conn.processRequest()){
                        goto done;
                    }
                }
            }
        }
    }

    done:
    http::Request & request = conn.getRequest();
    cout << "method: " << request.getMethod() << endl;
    cout << "url: " << request.getUrl() << endl;
    cout << "version: " << request.getVersion() << endl;
    cout << "parameters: " << endl;
    for(auto &key : request.getParameterNames()){
        cout << key << " " << request.getParameter(key) << endl;
    }

    out:
    conn.close();
    server.closeSocket();
    cout << "bye." << endl;

    return 0;
}