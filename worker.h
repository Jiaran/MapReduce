#ifndef WORKER_H

#define WORKER_H
#include "node.h"
#include <iostream>
class CWorker: public Node{
 public:
  CWorker( const char *, const char*);
  void join(NodeInfo& n);
  virtual void processRequest(rio_t &, int);
  void run();
 private:
  int workerID;
};


#endif
