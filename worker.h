#ifndef WORKER_H

#define WORKER_H
#include "node.h"
#include <iostream>
#include "mapFunction.h"
#include "reduceFunction.h"

class CWorker: public Node{
 public:
  CWorker( const char *, const char*, NodeInfo&);
  void join();
  virtual void processRequest(rio_t &, int);
  void run();
 private:
  mapFunction myMapFunction;
  reduceFunction myReduceFunction;
  NodeInfo master;
  int masterfd;
  int workerID;
  void complete(int id);
};


#endif
