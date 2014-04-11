#ifndef FILESYSTEM_H

#define FILESYSTEM_H



#include <string>
#include <iostream>
#include <map>
#include <stdlib.h>

// class FileSystem{
//  public:
//   FileSystem();
  
// }

//  communicate with Kang yi

typedef struct image_info
{
	//cv::Mat image_mat; // not sure
	int id;
	string filename;	
}image_info;


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




#endif
