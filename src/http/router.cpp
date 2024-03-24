#include "router.h"

namespace http {

RouteTree<Router::Handler> Router::routeTree;

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

void Router::addHandler(const std::string &url, Handler handler){
    routeTree.add(url, handler);
}

void Router::route(Request &request, Response &response){
    Handler handler = routeTree.get(request.getUrl(), getDefaultHandler());
    handler(request, response);
}

};