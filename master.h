#ifndef MASTER_H
#define MASTER_H
#include <assert.h>
#include <vector>
#include "csapp.h"
#include "node.h"
#include <map>
#include <set>
enum {IDLE,INPROCESS,  COMPLETE};
enum{MAP,REDUCE,NONE};

class Worker;
class Increase;

class Job{
 public:
  int jobID;
  int state;
  int work;
  int start;
  int end;
  int workerID;
  std::vector<int> mapJobs;
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
  bool alive;
  std::vector<int> jobs;
};


class Strategy{
 public:
  virtual void invoke(int id, Worker& w)=0;
};


class Master: public Node{
  friend class Increase;
 public:
  
  Master(int num, const char* port,const char * ip);
  int createWorker(const char * ip, const char * port, int state, int work);
  virtual void run();
  virtual void processRequest(rio_t & , int);
  ~Master();
 private :
  bool isDone;
  pthread_mutex_t myMutex;
  pthread_mutex_t checkComplete;
  static int uniqueID;
  int numReduce ;
  std::map<int, Worker> workers;
  std::vector<Job> jobs;
  void assignWork(int total);
  bool assignMap(int workerID, int jobID, int start, int end);
  void iterateWorkers(Strategy & s);
  bool assignReduce(int workerID, int jobID , std::vector<int> v );
  void reduceJobComplete(int jobID);
  void done();
  void reAssign(Worker &);
  void assignMap(NodeInfo , int , int ,int);
  void assignReduce(NodeInfo, std::vector<int>, int);
  void  reAssignReduce(Worker * w, Job & j);
  void  reAssignMap(Worker * w, Job & j);
  void modify(NodeInfo ,int , NodeInfo);
};


#endif
