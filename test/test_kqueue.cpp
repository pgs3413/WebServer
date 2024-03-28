#ifndef MAC
#define MAC
#endif

#include"../src/epoll/epoller.h"
#include"../src/socket/socket.h"
#include<iostream>
#include<assert.h>
#include<errno.h>
#include<unistd.h>

using namespace std;

int main(){

  cout << boolalpha;
  Epoller epoller(5);
  ServerSocket server(8080);
  cout << "add server event, fd: " << (int)server << endl;
  assert(epoller.addFd(server, true, false, false, false, false));
  Socket socket;

  cout << "listen..." << endl;
  assert(epoller.wait(-1));  
  for(auto event : epoller){
    if(event.getFd() == (int)server){
      socket = server.acceptSocket();
      cout << "accepted, fd: " << (int)socket << endl;
    }else {
      cout << "wrong listen" << endl;
      return 1;
    }
  }

  cout << "add read event, fd: " << (int)socket << endl;
  assert(epoller.addFd(socket, true, false, true, false, true));


  while (true)
  {
    cout << "wait..." << endl;
    bool result = epoller.wait(-1);
    if(!result){
      cout << "wait event failed" << endl;
      return 1;
    }
    for(auto event : epoller){
      cout << "fd: " << event.getFd() << " "
      << "read: " << event.isIn() << " "
      << "write: " << event.isOut() << " "
      << "close: " << event.isClose() << " "
      << "err: " << event.isErr()
      << endl;
      if(event.isClose()) goto out;
      result = epoller.modFd(event.getFd(), true, false, true, false, true);
      // result = epoller.delFd(event.getFd());
      if(!result){
        cout << "wrong" << endl;
        perror(nullptr);
        return 1;
      }
      close(socket);
    }

  }

  out:
  cout << "bye." << endl;
  return 0;
}