#include "reduceFunction.h"
#include <iostream>
#include <stdio.h>
//using namespace std;



////////////////////////////////////////////////////////
void reduceFunction::getMapInfo(){
for(std::map<int,mapNode>::iterator it=mapAddress.begin(); it!=mapAddress.end(); ++it){
	int fd;
	//int cound=0;
	while(true){
		fd=open_clientfd(it->second.mapIp, it->second.mapPort);
		if(fd<0){
			/*if(cound==3){
	NodeInfo newNode("8080", "10.0.2.15");
	changeInfo(1, newNode);
			}*/
			printf("Node is missing.\n"); 
			//sleep(1);
			//cound++;
			continue;	
      		}
		else{
			break;
		}
	}

		char buf_s[80];
		sprintf(buf_s,"GETMAP %d\n",it->first);
      		Rio_writep(fd, buf_s, strlen(buf_s));
		char buf_r[80];
		char * saveptr=NULL;
		int numBytes;
		rio_t t;
		Rio_readinitb( &t,fd);
		//std::cout<<"start while"<<std::endl;
		while(true){
  			numBytes = Rio_readlineb(&t, buf_r, MAXLINE);
  			if(numBytes==0){
					//std::cout<<"close fd"<<std::endl;
					Close(fd);
					break;
  			}
  			char* r1 = strtok_r(buf_r, "*",&saveptr);
			char r_name[80];
			strcpy(r_name,r1);
			std::string s_name(r_name);
			//std::cout<<s_name<<std::endl;
			char* r2 = strtok_r(NULL, "*",&saveptr);
			int r_num=atoi(r2);
			//std::cout<<" aaa"<<r_num<<std::endl;
			myMap[s_name]+=r_num;
			//std::cout<<r_num<<std::endl;
		}
	}
}
/////////////////////////////////////////////////////////////
void reduceFunction::sendInfo(){
	/*int fd=Open_clientfd(dataSystemIp, dataSystemPort);
	if(fd<0){
		printf("data system is missing.\n"); 
		return;	
	}
	char buf[80];*/
	printf("print result:\n");
	for (std::map<std::string,int>::iterator it=myMap.begin(); it!=myMap.end(); ++it){
		printf("%s: %d\n",it->first.data(),it->second);
		//sprintf(buf,"%s*%d\n",it->first.data(),it->second);
      		//Rio_writep(fd, buf, strlen(buf));
	}
	printf("print node addresses:\n");
	for (std::map<int, mapNode>::iterator nit=mapAddress.begin(); nit!=mapAddress.end(); ++nit){
		printf("%d: %s %d\n",nit->first, nit->second.mapIp, nit->second.mapPort);
		//sprintf(buf,"%s*%d\n",it->first.data(),it->second);
      		//Rio_writep(fd, buf, strlen(buf));
	}


}

////////////////////////for test///////////////////////
void reduceFunction::sendInfo(char* tip, int tport){
	/*int fd=Open_clientfd(tip, tport);
	if(fd<0){
		printf("data system is missing.\n"); 
		return;	
	}
	char buf[80];*/
	//std::cout<<"sendInfo"<<std::endl;
	for (std::map<std::string,int>::iterator it=myMap.begin(); it!=myMap.end(); ++it){
		std::cout<<it->first.data()<<" "<<it->second<<std::endl;
	}

}

/////////////////////////////////////////////////////
void reduceFunction::getInfo(int fd, rio_t client, int id){
	char buf1[MAXLINE];
	int numBytes;
  	char * saveptr=NULL;
	
	while(true){
  		numBytes = Rio_readlineb(&client, buf1, MAXLINE);
		if(numBytes==0){
    		break;
  		}
		printf("buf1 is %s\n", buf1);
		char* d1 = strtok_r(buf1, "*",&saveptr);
		char ip[20];
		strcpy(ip,d1);
		char* d2 = strtok_r(NULL, "*",&saveptr);
		int port=atoi(d2);
		char* d3 = strtok_r(NULL, "* \r\n",&saveptr);
		int num=atoi(d3);
		mapNode mapWorker;
		strcpy(mapWorker.mapIp,ip);
		mapWorker.mapPort=port;
		mapAddress[num]=mapWorker;
	}

	getMapInfo();
	sendInfo();

}
///////////////////////////////////////
void reduceFunction::changeInfo(int jobID, NodeInfo newNode){
	mapNode a;
	strcpy(a.mapIp,newNode.IP);
	a.mapPort=atoi(newNode.port);
	mapAddress[jobID]=a;
	//std::cout<<a.mapIp<<"  "<<a.mapPort<<std::endl;
}
////////////////////////////////////////
void reduceFunction::clearReduce(){
mapAddress.clear();
myMap.clear();
}

///////////////for test//////////////////
void reduceFunction::setIPNode(int tid, char* tip, int tport){
	mapNode a;
	strcpy(a.mapIp,tip);
	a.mapPort=tport;
	mapAddress[tid]=a;
}
///////////////////////////////////////////
void reduceFunction::printReduce(){
	printf("print result:\n");
	for (std::map<std::string,int>::iterator it=myMap.begin(); it!=myMap.end(); ++it){
		printf("%s: %d\n",it->first.data(),it->second);
	}
	printf("print node addresses:\n");
	for (std::map<int, mapNode>::iterator nit=mapAddress.begin(); nit!=mapAddress.end(); ++nit){
		printf("%d: %s %d\n",nit->first, nit->second.mapIp, nit->second.mapPort);
	}


}




