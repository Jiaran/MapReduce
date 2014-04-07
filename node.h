#ifndef NODE_H
#define NODE_H

#include "csapp.h"

class NodeInfo{
  public:
  char IP[20];
  char port[10];
  NodeInfo(const char *, const char *);
  NodeInfo();
};


class Node{

 protected:
  
 public:
  NodeInfo myInfo;
  Node(const char* port, const char * ip);
  virtual void run();
  virtual void processRequest(rio_t & , int)=0;
  void sendNodeInfo(int fd);
  NodeInfo readNodeInfo(rio_t & r);
  int connectTo(NodeInfo & n);
};

struct Arg {
  int connfd;
  Node * n;
  int myPort;
  char myIP[20];
} ;
void * receiveRequest(void * );
void * listenThread(void * );


#endif
