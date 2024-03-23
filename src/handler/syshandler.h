#ifndef SYS_HANDLER_H
#define SYS_HANDLER_H

#include "../http/request.h"
#include "../http/response.h"

void _404Handler(http::Request &req, http::Response &resp);

void _400Handler(http::Request &req, http::Response &resp);

#endif