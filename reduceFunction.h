#ifndef REDUCE_H
#define REDUCE_H


#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include "csapp.h"
#include <stdlib.h>
#include <vector>
#include <map>
#include "node.h"


struct mapNode{
	char mapIp[20];
	int mapPort;
};

class reduceFunction{
private:
char  dataSystemIp[20];
int dataSystemPort;
std::map<int, mapNode> mapAddress;
std::map<std::string,int> myMap;
public:
reduceFunction();
void getInfo(int fd, rio_t client, int id);
void getMapInfo();
void sendInfo();
void changeInfo(int jobID, NodeInfo newNode);
void clearReduce();
///////////////////for test/////////////////
void setIPNode(int tid, char* tip, int tport);
void sendInfo(char* tip, int tport);
void printReduce();

};

#endif
