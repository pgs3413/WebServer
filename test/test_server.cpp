#include "../src/server/server.h"

int main(){

    setenv("resources_path", "../resources/", 1);
    setenv("cgi_path", "../resources/", 1);
    setenv("ws_path", "../resources/", 1);

    logger::init(logger::DEBUG, "", "", 1000, true, false);

    Server server(8081);

    server.start();

}
