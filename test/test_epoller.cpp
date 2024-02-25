#include "../src/epoll/epoller.h"
#include<iostream>
#include<cstring>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char *argv[]){

    if(argc < 2 || std::strcmp(argv[1], "--help") == 0){
        std::cout << argv[0] << " file..." << std::endl;
        return 0; 
    }

    Epoller epoller(5);
    char buf[1024];

    for(int i = 1; i < argc; i++){

        int fd = open(argv[i], O_RDONLY);
        if(fd == -1) {
            std::cerr << "Could not open " << argv[i] << std::endl;
            return 1;
        }
        std::cout << "Opened " << argv[i] << " on fd " << fd << std::endl;

        if(!epoller.addFd(fd, true, false, false, false, false)){
            std::cerr << "Could not add fd " << fd << " in epoller" << std::endl;
            return 1;
        }
    }

    int fdNum = argc - 1;

    while (fdNum > 0)
    {

        if(!epoller.wait(-1)) continue;

        for(auto event : epoller){

            int fd = event.getFd();

            std::cout << "Ready: " << fd << std::endl;
            std::cout << "  event: ";
            if(event.isIn()) std::cout << "canread ";
            if(event.isOut()) std::cout << "canwrite ";
            if(event.isClose()) std::cout << "close ";
            if(event.isErr()) std::cout << "error ";
            std::cout << std::endl;

            if(event.isIn()){
                int s = read(fd, buf, 1024);
                if(s == -1){
                    std::cerr << "Could not read fd " << fd << std::endl;
                    return 1;
                }
                buf[s - 1] = '\0';
                std::cout << "  read " << s << " bytes: " << buf << std::endl;
            }

            if(event.isErr()){
                std::cout << "  closing fd " << fd << std::endl;
                epoller.delFd(fd);
                fdNum--;
            }
        }
    }


    std::cout << "All fd closed! Bye!" << std::endl;

    return 0;
}