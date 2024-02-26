#include "epoller.h"
#include<stdexcept>
#include<unistd.h>

Epoller::Epoller(int evListNum_){

    evListNum = evListNum_;
    eventNum = 0;
    epollFd = epoll_create1(0);
    if(epollFd == -1) 
        throw std::runtime_error("create epoller");
    evlist = new struct epoll_event[evListNum];

}

Epoller::~Epoller(){
    close(epollFd);
    delete [] evlist;
}

void Epoller::makeEvent(struct epoll_event * ev, bool in, bool out, bool close, bool et, bool oneshot){
    if(in) ev -> events |= EPOLLIN;
    if(out) ev -> events |= EPOLLOUT;
    if(close) ev -> events |= EPOLLRDHUP;
    if(et) ev -> events |= EPOLLET;
    if(oneshot) ev -> events |= EPOLLONESHOT;
}

bool Epoller::ctl(int fd, int op, bool in, bool out, bool close, bool et, bool oneshot){
    if(fd < 0) return false;
    struct epoll_event ev = {0};
    ev.data.fd = fd;
    makeEvent(&ev, in, out, close, et, oneshot);
    if(epoll_ctl(epollFd, op, fd, &ev) == -1) return false;
    return true;
}

bool Epoller::addFd(int fd, bool in, bool out, bool close, bool et, bool oneshot){
    return ctl(fd, EPOLL_CTL_ADD, in, out, close, et, oneshot);
}

bool Epoller::modFd(int fd, bool in, bool out, bool close, bool et, bool oneshot){
    return ctl(fd, EPOLL_CTL_MOD, in, out, close, et, oneshot);
}

bool Epoller::delFd(int fd){
    return ctl(fd, EPOLL_CTL_DEL, false, false, false, false, false);
}

bool Epoller::wait(int timeoutMs){
    eventNum = epoll_wait(epollFd, evlist, evListNum, timeoutMs);
    if(eventNum <= 0) return false;
    return true;
}


bool Epoller::Iterator::operator==(const Iterator &i){
    return epoller == i.epoller && index == i.index;
}

bool Epoller::Iterator::operator!=(const Iterator &i){
    return epoller != i.epoller || index != i.index;
}

Epoller::Item Epoller::Iterator::operator*(){
    Epoller::Item item ;
    if(index < 0 && index >= epoller -> eventNum) return item;
    item.fd = epoller -> evlist[index].data.fd;
    item.event = epoller -> evlist[index].events;
    return item;
}

Epoller::Iterator & Epoller::Iterator::operator++(){
    index++;
    return *this;
}
Epoller::Iterator Epoller::Iterator::operator++(int){
    Epoller::Iterator temp = *this;
    index++;
    return temp;
}

Epoller::Iterator Epoller::begin(){
    Epoller::Iterator i;
    i.epoller = this;
    i.index = 0;
    return i;
}

Epoller::Iterator Epoller::end(){
    Epoller::Iterator i;
    i.epoller = this;
    i.index = eventNum;
    return i;
}

bool Epoller::Item::isIn(){
    return event & EPOLLIN;
}

bool Epoller::Item::isOut(){
    return event & EPOLLOUT;
}

bool Epoller::Item::isClose(){
    return event & EPOLLRDHUP;
}

bool Epoller::Item::isErr(){
    return event & (EPOLLERR | EPOLLHUP);
}