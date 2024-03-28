#include "epoller.h"
#include<stdexcept>
#include<unistd.h>

#ifndef MAC

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

#else

Epoller::Epoller(int evListNum_){
  
  eventNum = 0;
  evListNum = evListNum_;
  kqFd = kqueue();
  if(kqFd == -1)
    throw std::runtime_error("create kqueuer");
  evlist = new struct kevent[evListNum];

}

Epoller::~Epoller(){
  close(kqFd);
  delete [] evlist;
}

void Epoller::makeEvent(struct kevent * ev, int fd, bool in, bool out, bool close, bool et, bool oneshot){
  short filter = 0;
  unsigned short flags = 0;
  if(in) filter |= EVFILT_READ;
  if(out) filter |= EVFILT_WRITE;
  if(close) flags |= EV_EOF;
  if(et) flags |= EV_CLEAR;
  if(oneshot) flags |= EV_ONESHOT;
  EV_SET(ev, fd, filter, flags | EV_ADD | EV_ENABLE, 0, 0, (void *)((long)fd));
}

bool Epoller::addFd(int fd, bool in, bool out, bool close, bool et, bool oneshot){
  if(fd < 0) return false;
  struct kevent event;
  makeEvent(&event, fd, in, out, close, et, oneshot);
  int result = kevent(kqFd, &event, 1, nullptr, 0, nullptr);
  if(result == -1) return false;
  return true;
}

bool Epoller::modFd(int fd, bool in, bool out, bool close, bool et, bool oneshot){
  if(fd < 0) return false;
  return addFd(fd, in, out, close, et, oneshot);
}

bool Epoller::delFd(int fd){
  if(fd < 0) return false;
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  int result = kevent(kqFd, &event, 1, nullptr, 0, nullptr);
  if(result == -1) return false;
  return true;
}

bool Epoller::wait(int timeoutMs){
  struct timespec time;
  if(timeoutMs > 0){
    time.tv_sec = timeoutMs / 1000;
    time.tv_nsec = (timeoutMs % 1000) * 1000000;
  }
  eventNum = kevent(kqFd, nullptr, 0, evlist, evListNum, timeoutMs > 0 ? &time : nullptr);
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
    item.fd = (long)(epoller -> evlist[index].udata);
    item.filter = epoller -> evlist[index].filter;
    item.flags = epoller -> evlist[index].flags;
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
    return filter & EVFILT_READ;
}

bool Epoller::Item::isOut(){
    return filter & EVFILT_WRITE;
}

bool Epoller::Item::isClose(){
    return flags & EV_EOF;
}

bool Epoller::Item::isErr(){
    return flags & EV_ERROR;
}

#endif