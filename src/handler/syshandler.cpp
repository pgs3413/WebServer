#include "syshandler.h"
#include "string"

using namespace http;

void _404Handler(Request &req, Response &resp){

    resp.setStatus(Response::NOT_FOUND);
    resp.setHeader("Content-Type", "text/html");

    std::string body;
    body += "<html>\n";
    body += " <title>404 Not Found</title>\n";
    body += " <body>\n";
    body += "  <h1>404 Not Found</h1>\n";
    body += "  <p>Sorry, the page ";
    body += req.getUrl();
    body += " you are looking for could not be found.</p>\n";
    body += " </body>\n</html>\n";

    resp.write(body); 

}

void _400Handler(Request &req, Response &resp){

    resp.setStatus(Response::BAD_REQUEST);
    resp.setHeader("Content-Type", "text/html");

    std::string body;
    body += "<html>\n";
    body += " <title>400 Bad Request</title>\n";
    body += " <body>\n";
    body += "  <h1>400 Bad Request</h1>\n";
    body += "  <p>Sorry, your request cannot be processed due to a bad syntax.</p>\n";
    body += " </body>\n</html>\n";

    resp.write(body); 
}