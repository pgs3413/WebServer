#include "cgihandler.h"
#include"../http/router.h"
#include<cstdlib>
#include<string>
#include<fcntl.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/wait.h>
#include<cstring>

void CGIHandler(http::Request &request, http::Response &response){

  char *cgi_path = std::getenv("cgi_path");
    std::string cgiPath = cgi_path == nullptr ? "./cgi_bin" : cgi_path;
    if(cgiPath[cgiPath.size() - 1] == '/'){
        cgiPath.pop_back();
    }

  char *cgi_prefix = std::getenv("cgi_prefix");
  std::string cgiPrefix = cgi_prefix == nullptr ? "/cgi" : cgi_prefix;

  std::string url = request.getUrl();
  if(url.compare(0, cgiPrefix.size(), cgiPrefix) != 0){
    http::Router::getDefaultHandler()(request, response);
    return;
  }

  url = url.substr(cgiPrefix.size());

  std::string fileName = cgiPath + url;

  int reqFd[2] = {-1, -1};
  int respFd[2] = {-1, -1};

  if(pipe(reqFd) == -1 || pipe(respFd) == -1){
    http::Router::getErrHandler()(request, response);
    close(reqFd[0]);
    close(reqFd[1]);
    close(respFd[0]);
    close(respFd[1]);
    return;
  }

  char buf[1024];
  int len = 0;
  while ((len = request.getBody(buf, 1024)) > 0){
    write(reqFd[1], buf, len);
  }

  pid_t pid = fork();

  if(pid == -1){
    http::Router::getErrHandler()(request, response);
    close(reqFd[0]);
    close(reqFd[1]);
    close(respFd[0]);
    close(respFd[1]);
    return;
  }

  if(pid == 0){ //子进程

    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);

    dup2(reqFd[0], 0);
    dup2(respFd[1], 1);
    dup2(respFd[1], 2);

    for(int i = 3; i <= rlim.rlim_cur; i++){
      close(i);
    }

    std::strcpy(buf, fileName.c_str());
    char * argv[] = {buf, nullptr};

    execvp(fileName.c_str(), argv);

  }else {

    close(reqFd[0]);
    close(reqFd[1]);
    close(respFd[1]);

    waitpid(pid, nullptr, 0);

    len = 0;
    while ((len = read(respFd[0], buf, 1024)) > 0){
      response.write(buf, len);
    }
    
    response.setHeader("Content-Type", "text/html");

    close(respFd[0]);

  }
  
}