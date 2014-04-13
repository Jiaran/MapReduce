#ifndef MAPFUNCTION_H

#define MAPFUNCTION_H

#include "opencv/cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <map>
#include <iostream>
#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
using namespace std;
using namespace cv;

struct imageInfo{
	string name;
	Mat myPic;
};

class mapFunction{
private:
	map<int, map<string,int> > myMap;
	map<int, bool> finished;
	pthread_mutex_t mtx;
	int circleDetect(Mat image1);
	
public:
	mapFunction();
	void goMap(int fd, rio_t client, int ID);
	bool ifDone(int id);
	void getMap(int fd, int ID);
	imageInfo getImage(int fileID);
	void clearMap();
};

#endif
