#include "chord.h"
#include "timer.h"


/////////////////kangyi//////////////////

extern Node myNode;
extern char preFlag;
extern Node fingerTable[];

Node myNode;
char * myIP = NULL;
int myPort = 0;
char * contactIP= NULL;
int contactPort=0;
char preFlag=0;
void timer_notify(void);
void fix_fingers(void);
SHA1Context  sc ;
Node fingerTable[32];


void stablize(){
  int fd=Open_clientfd(myNode.suc.ipAddress, atoi(myNode.suc.port));
  if(fd<0){
    setInfo(&myNode.suc,&myNode.sucsuc);
    keepAlive_s();
    return;
  }
  Node n= send_find_predecessor(atoll(myNode.suc.address),fd,0);
  if(atoi(n.pre.port)!=0&&isInRange(atoll(myNode.myInfo.address), atoll(myNode.suc.address), atoll(n.pre.address))){
    if(atoll(myNode.myInfo.address)!=atoll(n.pre.address)){
      setInfo(&myNode.suc,&n.pre);
      setInfo(&myNode.sucsuc,&n.myInfo);
    }
    else{
      setInfo(&myNode.sucsuc,&n.suc);
    }
    
  }
  keepAlive_s();
}


Node closest_preceding_finger(unsigned id, unsigned start){
	unsigned startKey;
	int i;
	for(i=31;i>=0;i--){
		if(!strcmp(fingerTable[i].myInfo.address, myNode.myInfo.address)){
			continue;
		}
		startKey=atoll(fingerTable[i].myInfo.address);
		if(isInRange(startKey,start,id)){
			if(atoi(fingerTable[i].sucsuc.port)==0){
				Node x ;
    				setInformation(&x.pre, myNode.myInfo.ipAddress, myNode.myInfo.port, myNode.myInfo.address);
    				setInformation(&x.myInfo, myNode.suc.ipAddress, myNode.suc.port, myNode.suc.address);
    				setInformation(&x.suc, myNode.sucsuc.ipAddress, myNode.sucsuc.port, 		myNode.sucsuc.address);
    				setInformation(&x.sucsuc, "0", "0", "0");
				return x;
			}
			else{
				return fingerTable[i];
			}
				
		}
	}
	Node x ;
    	setInformation(&x.pre, myNode.myInfo.ipAddress, myNode.myInfo.port, 		myNode.myInfo.address);
    	setInformation(&x.myInfo, myNode.suc.ipAddress, myNode.suc.port, myNode.suc.address);
    	setInformation(&x.suc, myNode.sucsuc.ipAddress, myNode.sucsuc.port, 		myNode.sucsuc.address);
    	setInformation(&x.sucsuc, "0", "0", "0");
	return x;
}



void keepAlive_s(){
  char buf[80];
  int fd=Open_clientfd(myNode.suc.ipAddress, atoi(myNode.suc.port));
  if(fd<=0)
    return;
  sprintf(buf,"NOTIFY\n");
  Rio_writep(fd, buf, strlen(buf));
  sendNodeInfo(fd, 32);
  Close(fd);
  
  
}

void keepAlive_r(Node n){
  if(strcmp(n.myInfo.address, myNode.pre.address)==0){
    preFlag=0;
  }
  
  if(atoi(myNode.pre.port)==0 || isInRange(atoll(myNode.pre.address),atoll(myNode.myInfo.address),atoll(n.myInfo.address))){
    setInfo(&myNode.pre,&n.myInfo);
  }
}


void createNode_N(){
  int fd=Open_clientfd(contactIP, contactPort);
	printf("------------joining------------\n");
  join(fd);
}


void ignore(){
}

void *processRequest(void* args)
{
  int numBytes, lineNum, serverfd, clientfd, serverPort,k;
  int byteCount = 0;
  char buf1[MAXLINE], buf2[MAXLINE];
  char host[MAXLINE];
  rio_t client;
  char * saveptr=NULL;
  char * cmd;
  clientfd = *(int *)(args);
	int i;
       
  Rio_readinitb( &client,clientfd);
  numBytes = Rio_readlineb(&client, buf1, MAXLINE);
  
  cmd = strtok_r(buf1, " \r\n",&saveptr);

  if(cmd==NULL){
    printf("yalo!\n");
    Close(clientfd);
    return;
  }
  if(strcmp(cmd, "HI")==0){

    for(k=0;k<32;k++){
      sendNodeInfo(clientfd,k);
    }
    
      
  }
  if(strcmp(cmd, "FIND")==0){
    

    unsigned id = atoll(strtok_r(NULL, " \r\n",&saveptr));
    receive_find_predecessor(id, clientfd);
    
      
  }
  if(strcmp(cmd, "NOTIFY")==0){
    Node n = readNode(clientfd,&client);
    keepAlive_r(n);  
    free(args);
    return;
  }
  Close(clientfd);
  free(args);
}




unsigned hash(char * key, int keyLength){
  int k=0;
  SHA1Reset(&sc);
  SHA1Input(&sc,key,keyLength);
  unsigned ans;
  if(SHA1Result(&sc)){
    ans= sc.Message_Digest[0];
    for(k=1; k< 5;k++)
      ans= ans^ sc.Message_Digest[k];
    
  }
  return ans;
}


void sendInfo(int clientfd, Info *n){
  char buf[80];
  sprintf(buf,"%s*%s*%s\n",n->ipAddress,n->port,n->address);
  Rio_writep(clientfd, buf, strlen(buf)); 
  
}


void sendNodeInfo( int clientfd, int id){
  if(id==32){
    sendNodeInfo_n(clientfd, &myNode);
  }
  else{
    sendNodeInfo_n(clientfd, &fingerTable[id]);
  }
}

void sendNodeInfo_n(int clientfd, Node * n){
  sendInfo(clientfd, &n->pre);
  sendInfo(clientfd, &n->myInfo);
  sendInfo(clientfd, &n->suc);
  sendInfo(clientfd, &n->sucsuc);
}


void setInfo( Info * i1 , Info * i2){
  strcpy(i1->ipAddress, i2->ipAddress); 
  strcpy(i1->port, i2->port); 
  strcpy(i1->address, i2->address); 
}


void setInformation(Info * n, char * ip, char * port, char * ad ){
  strncpy(n->ipAddress,ip,16);
	n->ipAddress[15]=0;
  strncpy(n->port,port,10);
	n->port[9]=0;
  strncpy(n->address,ad,20);
	n->address[19]=0;
}



void receive_find_predecessor(unsigned id, int fd){
  unsigned in=(unsigned)atoll(myNode.myInfo.address);
  unsigned suc=(unsigned)atoll(myNode.suc.address);
	if(isInRange(in,suc,id)){
     		sendNodeInfo(fd, 32);
  	}
	else{
		Node x=closest_preceding_finger(id, atoll(myNode.myInfo.address));
		sendNodeInfo_n(fd, &x);
	}
}


void print(Node * n){

  printf("pre info: %s, %s, %s \n", n->pre.ipAddress,n->pre.port,n->pre.address);
  printf("my info: %s, %s, %s \n", n->myInfo.ipAddress,n->myInfo.port,n->myInfo.address);
  printf("suc info: %s, %s, %s \n", n->suc.ipAddress,n->suc.port,n->suc.address);
  printf("sucsuc info: %s, %s, %s \n", n->sucsuc.ipAddress,n->sucsuc.port,n->sucsuc.address);
      
}

int compare(unsigned a, unsigned left, unsigned right){
	if(right>left){
		if(a>left&&a<right) return 1;
		else return 0;
	}
	if(a>left||a<right) return 1;
	return 0;
}


void *listenThread(void* args){
  int listenfd,  clientlen, optval;
  struct sockaddr_in clientaddr;
  struct hostent *hp;
  char *haddrp;
  pthread_t tid;

  listenfd = Open_listenfd(myPort);
  optval = 1;
  
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)); 
  
  while(1) {
    clientlen = sizeof(clientaddr);
    int * connfd = (int *)malloc(sizeof(int));
    *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	int * args = connfd;
    hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
		       sizeof(clientaddr.sin_addr.s_addr), AF_INET);

    haddrp = inet_ntoa(clientaddr.sin_addr);
     

    Pthread_create(&tid, NULL, processRequest, (void*)args);
    Pthread_detach(tid);
  }
}

void createNode (){
  int i=0;
  Info temp= myNode.myInfo;
  setInformation(&myNode.pre, "0", "0","0");
  setInformation(&myNode.suc, temp.ipAddress, temp.port,temp.address);
  setInformation(&myNode.sucsuc, temp.ipAddress, temp.port,temp.address);
  for(i=0; i< 32 ; i++){
    
    temp= myNode.myInfo;
    setInformation(&fingerTable[i].myInfo, temp.ipAddress, temp.port,temp.address);
    temp=myNode.suc;
    setInformation(&fingerTable[i].suc, temp.ipAddress,temp.port,temp.address);
    temp=myNode.pre;
    setInformation(&fingerTable[i].pre, temp.ipAddress,temp.port,temp.address);
    temp=myNode.sucsuc;
    setInformation(&fingerTable[i].sucsuc, "0", "0","0");
  }
}



void timer_notify(){
  preFlag++;
  if(preFlag==3){
    setInformation(&myNode.pre, "0", "0","0");
  }
  stablize();
  int i;
  if(strcmp(myNode.suc.address,myNode.myInfo.address)){
    fix_fingers();
  }
}

void printInfo(Info * i, char * s){
  printf("%s",s);
  printf("the node with the IP %s, Port %s, Hash %s\n ",i->ipAddress, i->port, i->address);
}


void search(unsigned id){
	/*char *temp=str;
	int i=0;
	while(temp!=NULL) i++;
	usigned searchID=hash(str, i);*/
	printf("Hash Value is %u\n", id);
	if(isInRange(atoll(myNode.myInfo.address), atoll(myNode.suc.address), id)){
	printInfo(&myNode.suc, "The information is at my successor ");
	return;
	}
	else{
	Node tryNode=closest_preceding_finger(id, atoll(myNode.myInfo.address));
	int fd=Open_clientfd(tryNode.myInfo.ipAddress, atoi(tryNode.myInfo.port));
	//printInfo(&tryNode.myInfo, "visited ");
	Node found=find_successor(id, fd,1);
	if(found.valid==0)
	  return;
	printInfo(&found.myInfo,"The information is at ");
	}
	
}


void fix_fingers(){
	static int fix_number=0;
	int i;
	setInformation(&fingerTable[0].myInfo,myNode.suc.ipAddress, myNode.suc.port, myNode.suc.address);
	setInformation(&fingerTable[0].suc, myNode.sucsuc.ipAddress, myNode.sucsuc.port, myNode.sucsuc.address);
	fix_number=fix_number%32;
	for(i=fix_number;i<fix_number+8;i++){
		int fd=Open_clientfd(myNode.suc.ipAddress, atoi(myNode.suc.port));
		unsigned fixID=atoll(myNode.myInfo.address)+pow(2,i);

		Node x=find_successor(fixID, fd,0);
		setInformation(&fingerTable[i].myInfo, x.myInfo.ipAddress, x.myInfo.port, x.myInfo.address);
		setInformation(&fingerTable[i].pre, x.pre.ipAddress, x.pre.port, x.pre.address);
		setInformation(&fingerTable[i].suc, x.suc.ipAddress, x.suc.port, x.suc.address);
		setInformation(&fingerTable[i].sucsuc, x.sucsuc.ipAddress, x.sucsuc.port, x.sucsuc.address);
	}
	fix_number+=8;
}




Node send_find_predecessor(unsigned id, int fd, int print){
  char buf[80];
  int numBytes;
  Node ans;
  
  ans.valid=1;
  sprintf(buf,"FIND %u from%s\n",id,myNode.myInfo.address);
  Rio_writep(fd, buf, strlen(buf));

  ans= readNode(fd,NULL);
  ans.valid=1;
  
  while(1){
    unsigned in=atoll(ans.myInfo.address);
    unsigned suc=atoll(ans.suc.address);
    
    if(isInRange(in,suc,id)){
	if(print)
	printInfo(&ans.myInfo, "visited ");
      break;
    }
    else{
      if(print)
	printInfo(&ans.myInfo,"visited ");

      fd=Open_clientfd(ans.myInfo.ipAddress, atoi(ans.myInfo.port));

      if(fd<0){
	printf("Node is missing. Trying to stablize\n"); 
	ans.valid=0;
	return ans;
	
      }
      sprintf(buf,"FIND %u from%s\n",id,myNode.myInfo.address);
      Rio_writep(fd, buf, strlen(buf));
      
      ans= readNode(fd,NULL);
      ans.valid=1;
      
    }
	
  };
  return ans;
  
  
}

Node readNode(int fd, rio_t * c){
  
  char buf[80];
  Node ans;
  int numBytes;
  rio_t t;
  if(c==NULL){
    Rio_readinitb( &t,fd);
    c= &t;
  }
  
  
  
  numBytes = Rio_readlineb(c, buf, MAXLINE);
  if(numBytes<0){
    Close(fd);
    return ans;
  }
  buf[numBytes]=0;
  seperate(buf, &ans.pre);
  numBytes = Rio_readlineb(c, buf, MAXLINE);
  if(numBytes<0){
    Close(fd);
    return ans;
  }

  buf[numBytes]=0;
  seperate(buf, &ans.myInfo);
  numBytes = Rio_readlineb(c, buf, MAXLINE);
  if(numBytes<0){
    Close(fd);
    return ans;
  }

  buf[numBytes]=0;
  seperate(buf, &ans.suc);
  numBytes = Rio_readlineb(c, buf, MAXLINE);
  if(numBytes<0){
    Close(fd);
    return ans;
  }

  buf[numBytes]=0;
  seperate(buf, &ans.sucsuc);
  
  Close(fd);
  return ans;
}

void seperate(char * s, Info * i){
  char * saveptr;
  char* tok;
  
  tok=strtok_r(s, "*",&saveptr);
  if(tok==NULL){
    printf("failed to parse info\n");
    return;
  }
  strcpy(i->ipAddress,tok);
  tok=strtok_r(NULL, "*",&saveptr);
  if(tok==NULL){
    printf("failed to parse info\n");
    return;
  }

  strcpy(i->port,tok);
  tok=strtok_r(NULL, "*\n",&saveptr);
  if(tok==NULL){
    printf("failed to parse info\n");
    return;
  }

  strcpy(i->address,tok);
  
  
}


Node find_successor(unsigned id, int fd,int print){
  Node n=send_find_predecessor(id, fd, print);
  if(n.valid==0){
    printf("address not valid\n");
    return n;
  }
  
  int nfd=Open_clientfd(n.suc.ipAddress, atoi(n.suc.port));
  
  Node nn= send_find_predecessor(atoll(n.suc.address),nfd,0);
  if(nn.valid==0){
    printf("address not valid\n");
  }
    
  return nn;
}


int isInRange(unsigned in, unsigned suc, int id){
  if(in==suc)
    return 1;
  if(in > suc){
    if(id>=in || id<suc){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    if(id>=in && id<suc){
      return 1;
    }
    else{
      return 0;
    }
  }
}


void join(int fd){
  setInformation(&myNode.pre,"0","0","0") ;
  myNode.suc = find_successor(atoll(myNode.myInfo.address), fd,0).myInfo;
  
}


int main(int argc, char** argv){

  int k;
  sigset_t sig_pipe; 
  pthread_t tid;
  char buf[20];
  

  myIP= argv[1];
  myPort= atoi(argv[2]);
  if(argc > 3){
    contactIP= argv[3];
    contactPort=atoi(argv[4]);
  }
  int i=0;
  while(argv[2][i]!=0){
    i++;
  }
  int keyLength = 15 + i+1;
  
  char * key = (char *)malloc(sizeof(char)* keyLength);
  strncpy(key, myIP,15+1);
  strncat(key, argv[2],i+1);
  sprintf(buf,"%u",hash(key,keyLength));
  setInformation(&myNode.myInfo,  myIP, argv[2],buf);
  printf("my hash value is %s\n", myNode.myInfo.address);
  
  Signal(SIGPIPE, ignore);
  
  if(sigemptyset(&sig_pipe) || sigaddset(&sig_pipe, SIGPIPE))
    unix_error("creating sig_pipe set failed");

  if(sigprocmask(SIG_BLOCK, &sig_pipe, NULL) == -1)
    unix_error("sigprocmask failed");
  
  createNode();
  if(argc>3){
    createNode_N();
  }
  Pthread_create(&tid, NULL, listenThread, NULL);
  Pthread_detach(tid);
  start_timer(5000, timer_notify);
  int len = 80;
  char * buffer = malloc (sizeof(char)*len);
  char * copy=buffer;
  int once = 1;
  while(1){
    if(once){
      once=0;
      printf("--------------------------------------------\n");
      printf("input hash value to search\n");
	printf("type \"p\" to print my node information\n");
	printf("type \"t\" to print mt finger_table\n");
	printf("type \"quit\" to exit\n");
    }
    
    buffer= fgets(buffer,len,stdin);
    if(buffer==NULL){
      buffer=copy;
      continue;
    }
    if(strcmp(buffer,"quit\n")==0){
      break;
    }
    else if(strcmp(buffer,"p\n")==0){
      once=1;
      print(&myNode);
	
    }
    else if(strcmp(buffer,"t\n")==0){
      int i;
      once =1;
	for(i=0;i<32;i++){
		printf("table node %d  %s %s %s\n ", i, fingerTable[i].myInfo.ipAddress, fingerTable[i].myInfo.port, fingerTable[i].myInfo.address);
	}
    }
    else {
      once=1;
      unsigned id=atoll(buffer);
      search(id);
    }
          
    
    
  }
  free(key);
  free(buffer);
}



