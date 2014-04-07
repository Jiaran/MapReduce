#ifndef MASTER_H
#define MASTER_H

#include <vector>
#include "csapp.h"
#include "node.h"
#include <map>
#include <set>
enum {IDLE,INPROCESS,  COMPLETE};
enum{MAP,REDUCE,NONE};

class Worker;

class Job{
 public:
  int jobID;
  int state;
  int work;
  int start;
  int end;
  std::vector<Worker *> mapWorkers;
  Job(int jobID, int work ){
    this->jobID= jobID;
    this->work = work;
    start=0;
    end=0;
  }
};


class Worker {
 public:
  int workerID;
  NodeInfo info;
  int absentTime;
  std::vector<int> jobs;
};


class Strategy{
 public:
  virtual void invoke(int id, Worker& w)=0;
};

class Master: public Node{
  
 public:
  
  Master(int num, const char* port,const char * ip);
  int createWorker(const char * ip, const char * port, int state, int work);
  virtual void run();
  virtual void processRequest(rio_t & , int);
  ~Master();
 private :
  pthread_mutex_t myMutex; 
  static int uniqueID;
  int numReduce ;
  std::map<int, Worker> workers;
  std::vector<Job> jobs;
  void assignWork(int total);
  bool assignMap(int workerID, int jobID, int start, int end);
  void iterateWorkers(Strategy & s);
  bool assignReduce(int workerID, int jobID , std::vector<Worker *> v );
};


#endif
