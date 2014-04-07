
#include "master.h"
#include <string.h>
#include <pthread.h>
#include "csapp.h"
#include <iostream>
#include <string>
//using namespace std;


class Print : public Strategy{
public:
  std::vector<Job> * jobs;
  virtual void invoke(int id , Worker &w ){
    printf("worker is %s, %s , its ID is %d\n", w.info.IP, w.info.port,w.workerID);
    for(int i=0;i< w.jobs.size();i++){
      printf("job: %d start is %d, end is %d \n ", jobs->at(w.jobs[i]).jobID, jobs->at(w.jobs[i]).start, jobs->at(w.jobs[i]).end );
    }
  }
};




int Master :: uniqueID =1; 
Master::Master(int num, const char * port, const char * ip):Node(port,ip){
  numReduce = num;
  pthread_mutex_init(&myMutex, NULL);
}

int Master::createWorker(const char * ip, const char * port, int state, int work){
  Worker w;
  strncpy(w.info.IP, ip,20);
  strncpy(w.info.port, port,10);
  pthread_mutex_lock(&myMutex);
  w.workerID= uniqueID;
  workers.insert(std::pair<int ,Worker> (w.workerID, w));
  uniqueID++;
  pthread_mutex_unlock(&myMutex);
  return w.workerID;
}

void Master:: run(){
  Node::run();
  while(true){
    std::string s;
    std::cin>>s;
    
    if(s=="p"){
      Print p;
      p.jobs=&jobs;
      iterateWorkers(p);
     
    }
    else if(s=="q"){
      return;
    }
    else{
      int total = atoi(s.c_str());
      assignWork(total);
    }
  }
  
}
  


void Master:: processRequest(rio_t & client, int clientfd){
  char buf[MAXLINE];
  int numBytes;
  char * saveptr;
  char * cmd;
  numBytes = Rio_readlineb(&client, buf, MAXLINE);
  cmd = strtok_r(buf, " \r\n",&saveptr);
  
  if(cmd==NULL){   
    Close(clientfd);
    return;
  }
  else if(strcmp(cmd,"JOIN")==0){
    
    NodeInfo n= readNodeInfo(client);
    int id=createWorker(n.IP,n.port,IDLE,NONE);
    sprintf(buf,"%d/n",id);
    Rio_writep(clientfd, buf, strlen(buf));
  }
  Close(clientfd);
  
}




void Master:: assignWork(int total){
  
  int size = workers.size();
  if(size<= numReduce){
    std::cout<< "Not enough wokers. Please try again later"<<std::endl;
  }
  int numMap = size - numReduce;
  std::vector< std::vector<Worker *> >  temp;
  for(int i=0;i<numReduce;i++){
    temp.push_back(std::vector<Worker *> ());
  }
  int interval = total/numMap;
  int start =1 ;
  int current=1;
  int currentReduce=0;
  for (std::map<int,Worker>::iterator it=workers.begin(); it!=workers.end(); it++,current++){
    if(current< numMap){
      assignMap(it->second.workerID,current, start, start+interval-1);
      temp[(current-1)%numReduce].push_back(&it->second);
      start+=interval;
    }
    else if(current == numMap){
      assignMap(it->second.workerID,current, start, total);
      temp[(current-1)%numReduce].push_back(&it->second);
    }
    else{
      assignReduce(it->second.workerID,current, temp[currentReduce]);
      currentReduce++;
    }
  }
  
}


bool Master:: assignMap(int workerID, int jobID, int start , int end){
  Worker & w = (workers.find(workerID)->second);
  Job j(jobID,MAP);
  j.start = start;
  j.end = end;
  j.state = INPROCESS;
  jobs.push_back(j);
  w.jobs.push_back(jobs.size()-1);
 
  int fd =connectTo(w.info);
  char buf[MAXLINE];
  sprintf(buf,"MAP %d\n",jobID);
  Rio_writep(fd, buf, strlen(buf)); 
  sprintf(buf,"%d*%d\n",start,end);
  Rio_writep(fd,buf,strlen(buf));
  Close(fd);
}

bool Master:: assignReduce(int workerID, int jobID, std::vector<Worker *> v ){
  Worker & w = (workers.find(workerID)->second);
  Job j(jobID,REDUCE);
  j.state = INPROCESS;
  j.mapWorkers = v;
  jobs.push_back(j);
  w.jobs.push_back(jobs.size()-1);
  
  int fd =connectTo(w.info);
  char buf[MAXLINE];
  sprintf(buf,"REDUCE %d\n", jobID);
  Rio_writep(fd, buf, strlen(buf));
  for(int i=0; i< v.size();i++){
    sprintf(buf,"%s*%s\n",v[i]->info.IP, v[i]->info.port);
    Rio_writep(fd,buf,strlen(buf));
  }
  Close(fd);
} 


void Master:: iterateWorkers(Strategy &s){
  for (std::map<int,Worker>::iterator it=workers.begin(); it!=workers.end(); ++it){
    s.invoke(it->first, it->second);
  }
}
Master:: ~Master(){
  pthread_mutex_destroy(&myMutex);
}






