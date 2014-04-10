#ifndef MAPWORKER_H

#define MAPWORKER_H

#include <string>
#include <map>
#include <iostream>
#include "cv.h"
using namespace std;
using namespace cv;

struct imageInfo{
	string name;
	Mat myPic;
};


class mapWorker{
private:
	int fileStartID;
	int fileEndID;
	map<string,int> myMap;
	
public:
	mapWorker(int start, int terminal);

	imageInfo getImage(int fileID);
	map<string, int> getMap();
	int getStartID();
	int getEndID();
	
	

};

#endif
