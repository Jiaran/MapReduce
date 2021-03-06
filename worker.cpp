#include "worker.h"

extern NodeInfo FILE_SYSTEM;
class Update : public TimerProcess{
private:
  int fdtoMaster;
  int workerID;
public:
  Update(int fd , int id){
    fdtoMaster = fd;
    workerID=id;
  }
  virtual void invoke(){
    char buf[20];
    sprintf(buf,"UPDATE*%d\n",workerID);
    Rio_writep(fdtoMaster, buf, strlen(buf)); 
    
  }

};

CWorker:: CWorker(const char * port,const char * ip,NodeInfo& master ):Node(port,ip){
  this->master = master;
}

void CWorker:: run(){
  Node::run();
  join();
  while(true){
    std::string s;
    std::cin>>s;
    if(s=="q"){
      break;
    }
  
  }
  Close(masterfd);
}

void CWorker:: join(){
  int fd = connectTo(master);
  char buf[MAXLINE];
  sprintf(buf,"%s","JOIN\n");
  Rio_writep(fd, buf, strlen(buf)); 

  sendNodeInfo(fd);
  
  int numBytes=Rio_readp(fd, buf, MAXLINE);
  buf[numBytes]=0;
  workerID=atoi(buf);
  printf("buf is ::::%s,,,workerID is %d\n",buf,workerID);
  masterfd = fd;
  timerProcess = new Update( masterfd,workerID);
  timerStart();

}

void CWorker:: processRequest(rio_t & client, int clientfd){
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
  else if(strcmp(cmd,"MAP")==0){
    cmd = strtok_r(NULL, " \r\n",&saveptr);
    int jobID = atoi(cmd);
    printf("MAP it is , job ID %s\n",cmd);
    
    //handle map work;//////////////////////////
    myMapFunction.goMap(clientfd, client,jobID);
  }
  else if(strcmp(cmd,"REDUCE")==0){
    
    cmd = strtok_r(NULL, " \r\n",&saveptr);
    printf("REDUCE , job ID %s\n",cmd);
    int jobID = atoi(cmd);
    /*
    while(numBytes = Rio_readlineb(&client, buf, MAXLINE)>0 ){
      printf("buf is %s\n",buf);
    }
    */
    //handle reduce work/////////////////////
    myReduceFunction.getInfo(clientfd, client, jobID);
   
    complete(jobID);
    
  }
  else if(strcmp(cmd,"DONE")==0){
    //clear Map and Reduce;///////////////////////
    myMapFunction.printMap();
    myMapFunction.clearMap();
    myReduceFunction.clearReduce();
    printf("done\n");
  }
  else if(strcmp(cmd,"MODIFY")==0){
    //MODIFY JOBID\n IP*PORT
    cmd = strtok_r(NULL, " \r\n",&saveptr);
    int jobID =atoi(cmd);
    NodeInfo n= readNodeInfo(client);
    /////////////////////////////////
    myReduceFunction.changeInfo(jobID, n);
    printf("MODIFY jobID is %d, ip %s, port %s\n",jobID, n.IP, n.port);
  }
  else if(strcmp(cmd,"GETMAP")==0){
    cmd = strtok_r(NULL, " \r\n",&saveptr);
    int jobID =atoi(cmd);
    //call get map
    myMapFunction.getMap(clientfd,jobID);

  }

  Close(clientfd);
  
}


void CWorker:: complete(int jobID){
  int fd = connectTo(master);
  char buf[MAXLINE];
 
  sprintf(buf,"COMPLETE\n%d*%d\n",jobID,workerID);
  Rio_writep(fd, buf, strlen(buf)); 
  Close(fd);


} 
