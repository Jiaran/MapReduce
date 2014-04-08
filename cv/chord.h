#ifndef CHORD_H

#define CHORD_H

#include "csapp.h"
#include <stdlib.h>
#include "sha1.h"
#include <stdio.h>
#include <math.h>


typedef struct Info{
  char  ipAddress[16];
  char port[10];
  char address[20];
} Info;
typedef struct Node {
  Info myInfo;
  Info pre;
  Info suc;
  Info sucsuc;
  int valid;
} Node;

Node send_find_predecessor(unsigned id, int fd,int print);
void receive_find_predecessor(unsigned id, int fd);
Node find_successor(unsigned id, int fd,int print);
void join(int fd);
Node readNode(int fd, rio_t * c);
void seperate(char *s, Info * i);
void keepAlive_r(Node n);
void keepAlive_s();
void setInfo(Info * i1, Info * i2);
void stablize();
void print(Node *);
int compare(unsigned a, unsigned left, unsigned right);
Node closest_preceding_finger(unsigned id, unsigned start);
void createNode();
void createNode_N();
void quit();
void search(unsigned id);
void * processRequest(void * args );
void * listenThread(void * args );
void sendNodeInfo(int clientfd, int id);
void ignore();
unsigned hash(char * key, int keyLength);
void setInformation(Info * n, char * ip, char * port, char * ad );
void sendInfo(int clientfd, Info *n );
void sendNodeInfo_n(int clientfd, Node * n);
void printInfo(Info * i, char * s);


#endif
