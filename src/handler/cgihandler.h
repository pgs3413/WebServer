#ifndef CGI_HANDLER_H
#define CGI_HANDLER_H

#include "../http/request.h"
#include "../http/response.h"


void CGIHandler(http::Request &, http::Response &);

#endif