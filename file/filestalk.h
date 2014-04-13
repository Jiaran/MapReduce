
#ifndef FILESTALK_H

#define FILESTALK_H

#include <string>
#include <stdlib.h>
#include <dirent.h>   // dir
#include <ctype.h>  // isdigit
#include <iostream>
#include <map>

using namespace std;

void ignore();

void *peertalk(void* args);
void *listening(void* args);

typedef struct image_info
{
	//cv::Mat image_mat; // not sure
	int id;
	string filename;	
}image_info;

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
	image_info* NumtoInfo(int id);

};

void printimage_info(image_info *res);


#endif
