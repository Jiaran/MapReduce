
#include "filestalk.h"

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
  	dir = opendir("./image");   // open current directory
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


imageInfo FileDir::NumtoInfo(int id){
	map<int, string>::iterator it;
	it = mymap.find(id);
	imageInfo result;
	result.name=it->second;
	char namename[80];
	sprintf(namename, "image/%d.%s.jpg",it->first,it->second.data());
	//string imageName;
	//strcpy(imageName,namename);
	//imageName=imageName+"."+it->second+".jpg";
	result.myPic=imread(namename,CV_LOAD_IMAGE_COLOR);
	return result;
}

