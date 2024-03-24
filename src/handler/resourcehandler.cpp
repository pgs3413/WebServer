#include "resourcehandler.h"
#include<cstdlib>
#include<string>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include"../http/router.h"

void ResourceHandler(http::Request &request, http::Response &response){

    char *resources_path = std::getenv("resources_path");
    std::string resourcesPath = 
        resources_path == nullptr ? "./resources" : resources_path;
    if(resourcesPath[resourcesPath.size() - 1] == '/'){
        resourcesPath.pop_back();
    }

    std::string fileName = resourcesPath + request.getUrl();

    int fd = open(fileName.c_str(), 0);
    if(fd == -1){
        http::Router::getDefaultHandler()(request, response);
        return;
    }

    struct stat statbuf;
    fstat(fd ,&statbuf);

    void *buf = mmap(nullptr, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(buf == MAP_FAILED){
        http::Router::getDefaultHandler()(request, response);
        close(fd);
        return;
    }

    response.write(buf, statbuf.st_size, [](void *_buf, size_t _bufSize){
        munmap(_buf, _bufSize);
    });

    close(fd);

}