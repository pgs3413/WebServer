#ifndef EPOLLER_H_
#define EPOLLER_H_

#include<sys/epoll.h>


class Epoller{

private:

    int epollFd;
    int evListNum;
    int eventNum;
    struct epoll_event * evlist;

    void makeEvent(struct epoll_event * ev, bool in, bool out, bool close, bool et, bool oneshot);
    bool ctl(int fd, int op, bool in, bool out, bool close, bool et, bool oneshot);

public:
    explicit Epoller(int evListNum_ = 1024);
    ~Epoller();
    Epoller(const Epoller &) = delete;
    Epoller & operator=(const Epoller &) = delete;

    bool addFd(int fd, bool in, bool out, bool close, bool et, bool oneshot);
    bool modFd(int fd, bool in, bool out, bool close, bool et, bool oneshot);
    bool delFd(int fd);
    bool wait(int timeoutMs = -1);

    class Item;

    class Iterator{

        Epoller * epoller;
        int index;

        public:

            friend class Epoller;

            Iterator(Epoller * epoller = nullptr, int index = 0) : epoller(epoller) , index(index){}
            ~Iterator() = default;
            bool operator==(const Iterator &i);
            bool operator!=(const Iterator &i);
            Item operator*();
            Iterator & operator++();
            Iterator operator++(int);

    };

    class Item{

        int fd;
        uint32_t event;

        public:

            friend class Iterator;

            Item(int fd = -1, uint32_t event = 0) : fd(fd) , event(event){}
            ~Item() = default;

            int getFd(){return fd;}
            bool isIn();
            bool isOut();
            bool isClose();
            bool isErr();

    };

    Iterator begin();
    Iterator end();

};

#endif