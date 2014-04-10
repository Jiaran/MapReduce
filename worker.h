#ifndef WORKER_H

#define WORKER_H
#include "node.h"
#include <iostream>
class CWorker: public Node{
 public:
  CWorker( const char *, const char*, NodeInfo&);
  void join();
  virtual void processRequest(rio_t &, int);
  void run();
 private:
  NodeInfo master;
  int masterfd;
  int workerID;
  void complete(int id);
};


#endif
