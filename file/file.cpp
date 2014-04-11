//http://blog.davidqiu.com/post/2012-09-24/40038004665

#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>
#include <dirent.h>   // dir
#include <ctype.h>  // isdigit
using namespace std;



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

FileDir::FileDir(){

}

void FileDir::Printfile(){
	DIR * dir;
  	dirent* pdir;

  	dir = opendir(".");   // open current directory
  	pdir = readdir(dir);
  	while (pdir) {
  		cout << pdir->d_name << endl;
  		pdir = readdir(dir);
        
    }
    closedir(dir);
}

void FileDir::Createmap(){
	// create a map stores the file under this folder.
	DIR * dir = NULL;
  	dirent* pdir = NULL;
  	char * saveptr = NULL;
  	char * cmd;
  	int id;
  	string filename;
  	char f_name[80];
  	//map < int, char* > mymap;
  	char name[80];
  	dir = opendir(".");   // open current directory
  	pdir = readdir(dir);
  	while (pdir) {
  		
        strcpy(name,pdir->d_name);
        
        if(strlen(name)>2){
        	// remove . and ..
        	cmd = strtok_r(name,".", &saveptr);
        	if(isdigit(cmd[0])) {
        		cout << pdir->d_name << endl;
        		id = atoi(pdir->d_name);
        		filename = strtok_r(NULL, ".", &saveptr);
        		cout << id << " " << filename << endl;
        		//strcpy(f_name,filename);
        		mymap.insert(pair< int, string >(id, filename));
        		//Printmap();
        	}
        }
        pdir = readdir(dir);
    }
    closedir(dir);
}

void FileDir::Printmap(){
	map<int, string>:: iterator it;
  	for (it = mymap.begin(); it!=mymap.end(); ++it)
  	{
  		cout << it->first << " => " << it->second << '\n';
  	}
}

// void PutinMap(int id, char* filename){
	
// 	mymap.insert(pair< int, char* >(id, filename));
// }

// // Initialize
// FileDir::FileDir(){
// 	// use current folder
// 	if(getcwd(init_filepath,sizeof(init_filepath)) == NULL)
// 		perror ("getcwd() error");
// 	int len = strlen(init_filepath);
// 	// if the last char is not /, add //
// 	if(init_filepath[len-1] != '//')
// 		strcat(init_filepath,"//");

// }

image_info* FileDir::NumtoInfo(int id){
	map<int, string>::iterator it;
	it = mymap.find(id);
	image_info *res = new image_info;
	res->id = it->first;
	res->filename = it->second;
	cout << res->id << ", " << res->filename << endl; 
	cout << "key :" << it->first << endl;
	cout << "value :"<< it->second << endl;
	return res;
}


void printimage_info(image_info *res){
	cout<< res->id << "=>" << res->filename <<endl;
}

int main(int argc, char *argv[])
{
	// map < int, char* > mymap;
	// char *a="XX";
	// mymap.insert(pair< int, char* >(1, a));
	// mymap.insert(pair< int, char* >(2, "chapel"));
	// mymap.insert(pair< int, char* >(3, "chapel"));
	// mymap.insert(pair< int, char* >(4332, "duke"));

	// mymap.emplace(1, "chapel");
	// mymap.emplace(2, "chapel");
	// mymap.emplace(3, "chapel");
	// mymap.emplace(4, "duke");


  	//unordered_map< int ,char* >::const_iterator got = mymap.find (input);
		// map<int, char*>:: iterator it;
  // 	for (it = mymap.begin(); it!=mymap.end(); ++it)
  // 	{
  // 		cout << it->first << " => " << it->second << '\n';
  // 	}


  	FileDir myfile;
  	myfile.Createmap();
  	myfile.Printmap();
  	image_info *res = myfile.NumtoInfo(3);
  	printimage_info(res);
	return 0;
}


