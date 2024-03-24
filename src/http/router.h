#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include<functional>
#include "request.h"
#include "response.h"
#include "../queue/routetree.h"
#include <string>

namespace http {

class Router {

public:
    typedef std::function<void(Request &, Response &)> Handler;

private:

    static Handler errHandler;
    static Handler defaultHandler;

    static RouteTree<Handler> routeTree;

public:

    static void setErrHandler(Handler handler);
    static Handler getErrHandler();

    static void setDefaultHandler(Handler handler);
    static Handler getDefaultHandler();

    static void addHandler(const std::string &url, Handler handler);
    static void route(Request &, Response &);
};

};

#endif