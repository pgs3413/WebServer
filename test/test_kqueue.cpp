#ifndef MAC
#define MAC
#endif

#include"../src/epoll/epoller.h"
#include<iostream>

using namespace std;

int main(){

  Epoller epoller(5);


  cout << "add event" << endl;
  bool result = epoller.addFd(0, true, false, false, false, false);
  
  if(!result){
    cout << "add event failed" << endl;
    return 1;
  }

  cout << "wait..." << endl;
  result = epoller.wait(-1);
  if(!result){
    cout << "wait event failed" << endl;
    return 1;
  }

  cout << boolalpha;
  for(auto event : epoller){
    cout << "fd: " << event.getFd() << " "
    << "can read: " << event.isIn() << " "
    << "can write: " << event.isOut() << " "
    << "close: " << event.isClose() << " "
    << "err: " << event.isErr()
    << endl;
  }

  return 0;
}