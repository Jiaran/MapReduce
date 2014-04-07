#include "worker.h"

CWorker:: CWorker(const char * port,const char * ip):Node(port,ip){
}

void CWorker:: run(){
  Node::run();
  while(true){
    std::string s;
    std::cin>>s;
    if(s=="q"){
      return;
    }
  
  }
}

void CWorker:: join(NodeInfo & n){
  int fd = connectTo(n);
  char buf[MAXLINE];
  sprintf(buf,"%s","JOIN\n");
  Rio_writep(fd, buf, strlen(buf)); 

  sendNodeInfo(fd);
  
  Rio_readp(fd, buf, MAXLINE);
  workerID=atoi(buf);
  int numBytes = Rio_readp(fd, buf, MAXLINE);
  Close(fd);

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
    
    printf("MAP it is , job ID %s\n",cmd);
    while(numBytes = Rio_readlineb(&client, buf, MAXLINE)>0){
      printf("%s\n",buf);

    }
    //handle map work;
  }
  else if(strcmp(cmd,"REDUCE")==0){
    
    cmd = strtok_r(NULL, " \r\n",&saveptr);
    printf("REDUCE , job ID %s\n ",cmd);
    while(numBytes = Rio_readlineb(&client, buf, MAXLINE)>0){
      printf("%s\n",buf);

    }
    //handle reduce work
    
  }
  Close(clientfd);
  
}
