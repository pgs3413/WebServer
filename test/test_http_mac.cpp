#include"../src/http/connection.h"
#include"../src/socket/socket.h"
#include<iostream>

using std::cout;
using std::endl;

int main(){

    ServerSocket server(8090);
    cout << "start to listen..." << endl;
    Socket socket = server.acceptSocket();
    cout << "listened." << endl;

    http::Connction conn(std::move(socket));

    while (!conn.processRequest()) ;
    
    http::Request & request = conn.getRequest();
    cout << "method: " << request.getMethod() << endl;
    cout << "url: " << request.getUrl() << endl;
    cout << "version: " << request.getVersion() << endl;

    conn.close();
    server.closeSocket();
    cout << "bye." << endl;

    return 0;
}