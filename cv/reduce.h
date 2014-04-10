#ifndef REDUCE_H
#define REDUCE_H


#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include "csapp.h"
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace cv;

class reduce{
private:
const char  dataSystemIp[16];
const int dataSystemPort;
vector<pair<char[16],int> > mapAddress;
public:
reduce();
void reduceInfo(int fd, rio_t client, int id);




};

#endifd
