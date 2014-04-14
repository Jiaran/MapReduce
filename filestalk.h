
#ifndef FILESTALK_H

#define FILESTALK_H

#include <string>
#include <stdlib.h>
#include <dirent.h>   // dir
#include <ctype.h>  // isdigit
#include <iostream>
#include <map>
#include <stdio.h>
#include "csapp.h"
#include <pthread.h>
#include "opencv/cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


struct imageInfo{
	string name;
	Mat myPic;
};

// The File System

class FileDir
{
protected:
	// store the filepath of our file system;
	char init_filepath[256];
	map < int, string > mymap;

public:
	FileDir();
	void Printfile();
	void Createmap();
	void Printmap();
	//void PutinMap(int id, char* filename);
	imageInfo NumtoInfo(int id);

};




#endif
