#include "../src/server/server.h"

int main(){

    log::init(log::DEBUG, "", "", 1000, true, false);

    Server server(8070);

    server.start();

}
