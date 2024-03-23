#include "router.h"

namespace http {



Router::Handler Router::errHandler = [](Request &req, Response &resp){};
Router::Handler Router::defaultHandler = [](Request &req, Response &resp){};

void Router::setErrHandler(Handler handler){
    Router::errHandler = handler;
}
Router::Handler Router::getErrHandler(){
    return Router::errHandler;
}

void Router::setDefaultHandler(Handler handler){
    Router::defaultHandler = handler;
}

Router::Handler Router::getDefaultHandler(){
    return Router::defaultHandler;
}


};