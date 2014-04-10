#ifndef MAPIMAGE_H

#define MAPIMAGE_H

#include <string>
#include <map>
#include <iostream>
#include "cv.h"
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

class mapImage{
private:
	map<int, map<string,int> > myMap;
	
public:
	mapImage(){
	}
	void goMap(int fd, rio_t client, int id){
		char buf1[MAXLINE];
		int jobID=id;
		int numBytes;
  		char * saveptr=NULL;
  		char * id1;
		char * id2;
		Rio_readinitb( &client,fd);
  		numBytes = Rio_readlineb(&client, buf1, MAXLINE);
		if(numBytes<0) return;
		id1 = strtok_r(buf1, " \r\n",&saveptr);
		int fileStartID=atoi(id1);
		id2 = strtok_r(buf1, " \r\n",&saveptr);
		int fileEndID=atoi(id2);
		map<string,int> jobMap;
		for(int i=fileStartID;i<=fileEndID;i++){
			imageInfo myImage=getImage(i);
			if(circleDetect(myImage.myPic)==0) continue;
			else{
				jobMap[myImage.name]=jobMap[myImage.name]+1;
			}
		}
		myMap[jobID]=jobMap;
		//Close(fd);
	}

	imageInfo getImage(int fileID){
		imageInfo result;
		result.name="shishi";
		result.myPic=imread("shishi.jpg",CV_LOAD_IMAGE_COLOR);
		return result;
	}

	void getMap(int fd, int ID){
		map<string,int> jobMap=myMap[ID];
		char buf[MAXLINE];
		for (map<string,int>::iterator it=jobMap.begin(); it!=jobMap.end(); ++it){
		sprintf(buf,"%s*%d\n",it->first.data(),it->second);
  		Rio_writep(fd, buf, strlen(buf));
		}
	}
};

#endif
