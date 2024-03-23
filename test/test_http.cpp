#include"../src/http/connection.h"
#include"../src/socket/socket.h"
#include<iostream>

using std::cout;
using std::endl;

int main(){

    ServerSocket server(8080);
    cout << "start to listen..." << endl;
    Socket socket = server.acceptSocket();
    cout << "listened." << endl;

    http::Connction conn(std::move(socket));

    conn.init();

    cout << "start to read..." << endl;

    while (!conn.processRequest());   
    
    http::Request & request = conn.getRequest();
    cout << "method: " << request.getMethod() << endl;
    cout << "url: " << request.getUrl() << endl;
    cout << "version: " << request.getVersion() << endl;
    cout << "parameters: " << endl;
    for(auto &key : request.getParameterNames()){
        cout << key << " " << request.getParameter(key) << endl;
    }
    cout << "headers: " << endl;
    for(auto &key : request.getHeaderNames()){
        cout << key << " " << request.getHeader(key) << endl;
    }
    cout << "body: " << endl;
    char buf[1024];
    std::string s;
    int len = 0;
    while ((len = request.getBody(buf, 1024)) > 0)
    {
        s.append(buf, len);
    }
    cout << s << endl;

    server.closeSocket();
    cout << "bye." << endl;

    return 0;
}