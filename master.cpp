
#include "master.h"
#include <string.h>
#include <pthread.h>
#include "csapp.h"
#include <iostream>
#include <string>
//using namespace std;

extern NodeInfo FILE_SYSTEM;
// strategy classes, no need to define in their own file. In fact it's just a function for Master.
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

class Done : public Strategy{
  
 public:
   virtual void invoke(int id , Worker &w ){
     char buf[5] = "DONE";
     int fd=Node::connectTo(w.info);
     Rio_writep(fd, buf, strlen(buf));
     Close(fd);
     w.jobs.clear();
  }
};

class Increase : public TimerProcess{
private:
  Master * master;
public:
  Increase(Master *m){
    master=m;
  }
  virtual void invoke(){
    pthread_mutex_lock(&master->myMutex);
    std::map<int,Worker>::iterator it=master->workers.begin();
    while(it!=master->workers.end()){
      if(!it->second.alive){
	it++;
	continue;
      }
      if(it->second.absentTime>=2){
	it->second.alive=false;
	printf("%d is offline\n",it->first);
	Worker broken(it->second);
	std::map<int,Worker>::iterator toErase = it;
	it++;
	master->workers.erase(toErase);
	master->reAssign(broken);
	
      }
      else{
	it->second.absentTime++;
	it++;
      }
    }
  
    pthread_mutex_unlock(&master->myMutex);
  }
};


// functions of Master

int Master :: uniqueID =1; 
Master::Master(int num, const char * port, const char * ip):Node(port,ip){
  numReduce = num;
  isDone=true;
  pthread_mutex_init(&myMutex, NULL);
  pthread_mutex_init(&checkComplete, NULL);
}

int Master::createWorker(const char * ip, const char * port, int state, int work){
  pthread_mutex_lock(&myMutex);
  Worker w;
  strncpy(w.info.IP, ip,20);
  strncpy(w.info.port, port,10);
  w.workerID= uniqueID;
  w.absentTime=0;
  w.alive=true;
  workers.insert(std::pair<int ,Worker> (w.workerID, w));
  uniqueID++;
  pthread_mutex_unlock(&myMutex);
  return w.workerID;
}

void Master:: run(){
  Node::run();
  timerProcess = new Increase( this);
  timerStart();
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
      if(!isDone){
	std::cout<<"In processing, try again later!"<<std::endl;
	continue;
      }
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
    sprintf(buf,"%d",id);
    Rio_writep(clientfd, buf, strlen(buf));
    while(numBytes=Rio_readlineb(&client, buf, MAXLINE)>0){

      printf("%s\n",buf);
      cmd = strtok_r(buf, "*",&saveptr);
      if(strcmp(cmd,"UPDATE")==0){
	cmd = strtok_r(NULL, "*",&saveptr);
	int workerID = atoi(cmd);
	workers[workerID].absentTime=0;
	printf("id %d is updating\n", workerID);
      }
    }
  }
  else if(strcmp(cmd, "COMPLETE")==0){
    numBytes=Rio_readlineb(&client, buf, MAXLINE);
    printf("buf is  %s\n", buf);
    cmd = strtok_r(buf, "*",&saveptr);
    int jobID = atoi(cmd);
    printf("id complete %s\n", cmd);
    if(jobs[jobID].work==REDUCE){
      reduceJobComplete(jobID);
    }
  }
   
  Close(clientfd);
  
}



void Master:: assignWork(int total){
  isDone=false;
  int size = workers.size();
  if(size<= numReduce){
    std::cout<< "Not enough wokers. Please try again later"<<std::endl;
    return;
  }
  int numMap = size - numReduce;
  std::vector< std::vector<int> >  temp;
  for(int i=0;i<numReduce;i++){
    temp.push_back(std::vector<int> ());
  }
  int interval = total/numMap;
  int start =1 ;
  int current=0;
  int currentReduce=0;
  for (std::map<int,Worker>::iterator it=workers.begin(); it!=workers.end(); it++,current++){
    if(current< numMap){
      assignMap(it->second.workerID,current, start, start+interval-1);
      temp[(current)%numReduce].push_back(current);
      start+=interval;
    }
    else if(current == numMap-1){
      assignMap(it->second.workerID,current, start, total);
      temp[(current)%numReduce].push_back(current);
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
  j.workerID = workerID;
  jobs.push_back(j);
  w.jobs.push_back(jobs.size()-1);
  assert(jobs.size()-1 == jobID);
  assignMap(w.info, start, end , jobID);
}

void Master:: assignMap(NodeInfo i, int start, int end, int jobID){
  int fd =connectTo(i);
  char buf[MAXLINE];
  sprintf(buf,"MAP %d\n",jobID);
  Rio_writep(fd, buf, strlen(buf)); 
  sprintf(buf,"%d*%d\n",start,end);
  Rio_writep(fd,buf,strlen(buf));
  Close(fd);
}

bool Master:: assignReduce(int workerID, int jobID, std::vector<int> v ){
  Worker & w = (workers.find(workerID)->second);
  Job j(jobID,REDUCE);
  j.state = INPROCESS;
  j.workerID = workerID;
  j.mapJobs = v;
  jobs.push_back(j);
  w.jobs.push_back(jobs.size()-1);
  assignReduce(w.info,v,jobID);
} 

void Master:: assignReduce(NodeInfo i , std::vector<int> v, int jobID){
  int fd =connectTo(i);
  char buf[MAXLINE];
  sprintf(buf,"REDUCE %d\n", jobID);
  Rio_writep(fd, buf, strlen(buf));
  for(int i=0; i< v.size();i++){
    Worker & w = workers[ jobs[v[i]].workerID ]; 
    sprintf(buf,"%s*%s*%d\n",w.info.IP, w.info.port,v[i]);
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



void Master:: reduceJobComplete(int id){
  pthread_mutex_lock(&checkComplete);
  
  jobs[id].state=COMPLETE;
  for(int i=0;i<jobs.size();i++){
    if(jobs[i].work==REDUCE&&jobs[i].state!=COMPLETE){
      pthread_mutex_unlock(&checkComplete);
      return;
    }
    
  }
  done();
  pthread_mutex_unlock(&checkComplete);
  
}


void Master:: done(){
  Done d;
  iterateWorkers(d);
  jobs.clear();
  int fd = connectTo(FILE_SYSTEM);
  char buf[5];
  sprintf(buf,"ALL\n");
  Rio_writep(fd, buf, strlen(buf));
  int numBytes;
  rio_t client;
  Rio_readinitb( &client,fd);
  printf("The track of the criminal is: \n");
  while(numBytes=Rio_readlineb(&client, buf, MAXLINE)>0){
    printf("%s\n",buf);
  }
  Close(fd);
  isDone=true;
  //output result send all to global file system and output;
  
}

void Master:: reAssign(Worker & broken){
  
  int workerID = broken.workerID;
  workers.erase(workerID);
  int min = 10000;
  Worker * nWorker = NULL;
  
  for (std::map<int,Worker>::iterator it=workers.begin(); it!=workers.end(); ++it){
    if(it->first == workerID)
      continue;
    else{
      if(it->second.jobs.size()<min){
	min=it->second.jobs.size();
	nWorker = &it->second;
	
      }
    }
  }
  for(int i=0; i<broken.jobs.size();i++){
    if(jobs[broken.jobs[i]].work==REDUCE&&jobs[broken.jobs[i]].state!=COMPLETE){
      reAssignReduce(nWorker, jobs[broken.jobs[i]]);
    }
  }
  for(int i=0; i<broken.jobs.size();i++){
    if(jobs[broken.jobs[i]].work==MAP){
      reAssignMap(nWorker, jobs[broken.jobs[i]]);
    }
  }
}

void Master:: reAssignReduce(Worker * w, Job & j){
  j.workerID = w->workerID;
  w->jobs.push_back(j.jobID);
  assignReduce(w->info , j.mapJobs, j.jobID);
}

void Master:: reAssignMap(Worker * w, Job & j){
  j.workerID = w->workerID;
  w->jobs.push_back(j.jobID);
  bool found =false;
  //find who is responsible for this map,Modify;
  for(int i=0;i<jobs.size();i++){
    if(found)
      break;
    if(jobs[i].work==REDUCE){
      for(int k=0; k< jobs[i].mapJobs.size();k++){
	if(jobs[i].mapJobs[k]==j.jobID){
	  modify(workers[jobs[i].workerID].info, j.jobID, w->info );
	  found = true;
	}
	break;
      }
    }
    
  }
  
    
  assignMap(w->info, j.start, j.end, j.jobID);
}

void Master:: modify(NodeInfo reduceNode, int jobID , NodeInfo newNode){
  int fd = connectTo(reduceNode);
  char buf[MAXLINE];
  sprintf(buf,"MODIFY *%d\n",jobID);
  Rio_writep(fd, buf, strlen(buf));
  sprintf(buf,"%s*%s\n",newNode.IP,newNode.port);
  Rio_writep(fd, buf, strlen(buf));
  Close(fd);
}
