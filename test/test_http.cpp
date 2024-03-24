#include"../src/http/connection.h"
#include"../src/socket/socket.h"
#include "../src/http/router.h"
#include "../src/handler/syshandler.h"
#include "../src/handler/resourcehandler.h"
#include<iostream>
#include<stdlib.h>

using std::cout;
using std::endl;

int main(){

    setenv("resources_path", "../resources/", 1);

    auto helloHandler = [](http::Request &req, http::Response &resp){
        // resp.setHeader("Content-Type", "text/plain");
        resp.write("Hello, World");
    };

    http::Router::setErrHandler(_400Handler);
    http::Router::setDefaultHandler(_404Handler);
    http::Router::addHandler("/hello", helloHandler);
    http::Router::addHandler("/*", ResourceHandler);

    ServerSocket server(8080);
    cout << "start to listen..." << endl;
    Socket socket = server.acceptSocket();
    cout << "listened." << endl;

    http::Connction conn(std::move(socket));

    int count = 1; // 测试一次keep-alive

    while (count >= 0)
    {
        conn.init();

        cout << "start to read..." << " (" << count << ")" << endl;

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

        cout << "start to write..." << " (" << count << ")" << endl;
        conn.processResponse();

        if(!conn.isKeepAlive()) break;
        count--;        
    }
    

    server.closeSocket();
    cout << "bye." << endl;

    return 0;
}