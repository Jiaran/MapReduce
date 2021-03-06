#include "filestalk.h"

pthread_mutex_t mutex;
int Localport;
void ignore();
void *peertalk(void* args);
void *listening(void* args);
void printimage_info(imageInfo *res);

map <string, int> place_map;

int main(int argc, char *argv[]){
  char * LocalIP = NULL;
  
  char buf[MAXLINE];
  char buf0[MAXLINE];
  pthread_t tid;
  int listenfd, listenPort, optval, clientlen, connfd;
  int serverfd, serverPort;
  pthread_mutex_init(&mutex, NULL);
  struct sockaddr_in clientaddr;
  struct hostent* hp;
  char *haddrp;
  sigset_t sig_pipe; 
  int sock;
  int* args;
  



  //  ./filesystem 127.0.0.1 8081
  LocalIP = argv[1];
  Localport = atoi(argv[2]);
  /* deal with SIGPIPE */
  printf("*********************** Filesystem starts! ******************\n");
  
  // Signal(SIGPIPE, ignore);
  
  // if(sigemptyset(&sig_pipe) || sigaddset(&sig_pipe, SIGPIPE))
  //   unix_error("creating sig_pipe set failed");

  // if(sigprocmask(SIG_BLOCK, &sig_pipe, NULL) == -1)
  //   unix_error("sigprocmask failed");


  Pthread_create(&tid, NULL, listening, NULL);
  Pthread_detach(tid);
  
  while(1){

  }
  pthread_mutex_destroy(&mutex);

}


void printimage_info(imageInfo *res){
	cout<< res->name <<endl;
}




void *listening(void* args){
  int listenfd, optval;
  pthread_t tid;
  /* important to use SO_REUSEADDR or can't restart proxy quickly */

  listenfd = Open_listenfd(Localport);
  printf("Localport is %d\n", Localport);
  printf("enter listening\n");
  optval = 1;
  
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)); 
  
  while(1) {
    int * connfd = (int *)malloc(sizeof(int));
    *connfd = Accept(listenfd, NULL, NULL);
    int * args = connfd;
     
    /* spawn a thread to process the new connection */
    
    Pthread_create(&tid, NULL, peertalk, (void*)args);
    Pthread_detach(tid);
  }

}




void *peertalk(void* args)
{
  int numBytes, serverfd, clientfd, serverPort;
  int byteCount = 0;
  char buf1[MAXLINE], buf2[MAXLINE];
  rio_t client;
  char * saveptr=NULL;
  char * cmd;
  clientfd = *(int *)(args);
    
  Rio_readinitb( &client,clientfd);
  numBytes = Rio_readlineb(&client, buf1, MAXLINE);
  printf("buf1 is %s\n", buf1);
  
  cmd = strtok_r(buf1, "\r\n",&saveptr);

  
  printf("cmd is %s\n", buf1);
  if(cmd==NULL){
    Close(clientfd);
    return NULL; 
  }
  else if(strcmp(cmd, "ADD")==0){
    pthread_mutex_lock(&mutex);
    printf("enter ADD\n");
    while(numBytes = Rio_readlineb(&client, buf1, MAXLINE)>0){
      map< string,int>::iterator it;
      char* place1 = strtok_r(buf1, "#",&saveptr);
      string place_s(place1);
      int times1 = atoi(strtok_r(NULL, "#",&saveptr));
      printf("place:%s, times:%d\n", place1,times1);
      it = place_map.find(place_s);
      if (it != place_map.end())
      {
        it->second += times1;
      }
      else{
        place_map.insert(pair<string, int>(place_s, times1));
	printf("place:%s, times:%d\n", place1,times1);
      }
	it=place_map.find("chapel");
	if(it!= place_map.end()){
		printf("chapel times is %d\n",it->second);
	}	
    }
    pthread_mutex_unlock(&mutex);
  }
  else if(strcmp(cmd, "ALL")==0){
    char place[10] = "chapel";
    char place1[10] = "Duke";
    int times = 9;
    
    pthread_mutex_init(&mutex, NULL);
    //while() iterator
    map< string,int>::iterator it;
    it = place_map.begin();
    while(it!=place_map.end()){
      sprintf(buf2,"%s#%d\n",it->first.c_str(),it->second);
      Rio_writep(clientfd,buf2,strlen(buf2));
      it++;
      printf("%s\n", buf2 );
    }
  }
  
  Close(clientfd);
  free(args);
}

void ignore() {
  ;
}
