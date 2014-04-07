#include "node.h"


NodeInfo:: NodeInfo(const char * p, const char * ip){
  strncpy(port,p,10);
  strncpy(IP, ip,20);
}
NodeInfo:: NodeInfo(){
}

Node:: Node(const char* port, const char * ip){
  
  strncpy(myInfo.port,port,10);
  strncpy(myInfo.IP, ip,20);
}

void Node:: run(){
  pthread_t tid;
  Arg * args= (Arg*)malloc(sizeof(Arg));
  args->n = this;
  args->myPort = atoi(myInfo.port);
  Pthread_create(&tid, NULL, listenThread, (void*)args);
  Pthread_detach(tid);
  
}

void *  listenThread(void * a){
  int listenfd, optval;
  pthread_t tid;
  Arg * myargs = (Arg *)a;
  /* important to use SO_REUSEADDR or can't restart proxy quickly */
  printf("%d\n",myargs->myPort);
  listenfd = Open_listenfd(myargs->myPort);
  optval = 1;
  
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)); 
  
  while(1) {
    Arg * args = (Arg *) malloc(sizeof(*args));
    
    args->connfd = Accept(listenfd, NULL, NULL);
    args->n = myargs->n;
    Pthread_create(&tid, NULL, receiveRequest, (void*)args);
    Pthread_detach(tid);
  }
  free(myargs);
  
}

void * receiveRequest(void * a){
  int clientfd;
  int byteCount = 0;
  rio_t client;
  char * saveptr=NULL;
  char * cmd;
  Arg * args = (Arg *)a;
  clientfd = args->connfd;	
  Rio_readinitb( &client,clientfd);
  args->n->processRequest(client,clientfd);
  free(args);
}

void Node:: sendNodeInfo(int fd){
  char buf[MAXLINE];
  sprintf(buf,"%s*%s\n",myInfo.IP,myInfo.port);
  Rio_writep(fd, buf, strlen(buf)); 
} 

NodeInfo Node:: readNodeInfo(rio_t & r){
  NodeInfo ans ;
  int numBytes;
  char * saveptr;
  char buf[MAXLINE];
  char * temp;
  numBytes = Rio_readlineb(&r, buf, MAXLINE);
  printf("h::%s\n",buf);
  temp = strtok_r(buf, "*\r\n",&saveptr);
  strncpy(ans.IP,temp,20);
  temp = strtok_r(NULL, "*\r\n",&saveptr);
  strncpy(ans.port,temp,10);
  
  return ans;
  
  
}

int Node:: connectTo(NodeInfo & n){
  int fd=Open_clientfd(n.IP, atoi(n.port));
  return fd;
}
