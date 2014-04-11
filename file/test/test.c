# include <stdio.h>
# include <string.h>
# include "csapp.h"




 int main(int argc, char *argv[])
{

  char *TargetIP = NULL;
  int Targetport;
  char buf[50];
  char readbuf[MAXLINE];
  pthread_t tid;
  int listenfd, listenPort, optval, clientlen, connfd;
  int serverfd, serverPort;
  
  int numbytes;
  struct sockaddr_in clientaddr;
  struct hostent* hp;
  char *haddrp;
  sigset_t sig_pipe; 
  int sock;
  int* args;
  int len = 50;



  TargetIP = argv[1]; 
  
  Targetport = atoi(argv[2]);


  connfd=open_clientfd(TargetIP, Targetport);
  printf("------------Connect to the TargetNdoe------------\n");
  printf("Chapel#5\n");
  char * buffer = malloc (sizeof(char)*len);
  while(1){
  //ADD STARTS
  bzero(buf, len);
  bzero(buffer,len);
  buffer = fgets(buffer,len,stdin);
  // Chapel#8
  sprintf(buf, "%s\r\n", buffer);
  printf("buf: %s\n", buf);
  
  // =============write into the pipe========
  rio_writep(connfd,buf,strlen(buf));
  
  //printf("readbuf :%s\n", readbuf);

  }


}
