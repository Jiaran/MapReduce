#include "mapFunction.h"


mapFunction::mapFunction(){
pthread_mutex_init(&mtx,NULL);
}

int mapFunction::circleDetect(Mat image1){
	IplImage* img = new IplImage(image1);
        IplImage* gray = cvCreateImage( cvGetSize(img), img->depth, 1);
        CvMemStorage* storage = cvCreateMemStorage(0);
        cvCvtColor( img, gray, CV_RGB2GRAY );
        cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 );
        CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
        if(circles->total==0) return 0;
	else return 1;
	}

/////////////////////////////////////////////////////////////////
	void mapFunction::goMap(int fd, rio_t client, int ID){
		pthread_mutex_lock(&mtx);
		char buf1[MAXLINE];
		int jobID=ID;
		int numBytes;
  		char * saveptr=NULL;
  		char * id1;
		char * id2;
		//char * id3;
		//Rio_readinitb( &client,fd);
  		numBytes = Rio_readlineb(&client, buf1, MAXLINE);
		if(numBytes<0) return;
		id1 = strtok_r(buf1, "*",&saveptr);
		int fileStartID=atoi(id1);
		id2 = strtok_r(NULL, "* \r\n",&saveptr);
		int fileEndID=atoi(id2);
		//id3 = strtok_r(NULL, "* \r\n",&saveptr);
		//jobID=atoi(id3);
		map<string,int> jobMap;
		for(int i=fileStartID;i<=fileEndID;i++){
			imageInfo myImage=getImage(i);
			if(circleDetect(myImage.myPic)==0) continue;
			else{
				jobMap[myImage.name]=jobMap[myImage.name]+1;
			}
		}
		myMap[jobID]=jobMap;
		finished[jobID]=true;
		pthread_mutex_unlock(&mtx);
		//Close(fd);
	}
///////////////////////////////////////////////////////////////
	imageInfo mapFunction::getImage(int fileID){
		imageInfo result;
		result.name="shishi";
		result.myPic=imread("shishi.jpg",CV_LOAD_IMAGE_COLOR);
		return result;
	}
///////////////////////////////////////////////////////////////
	bool mapFunction::ifDone(int id){
		if(finished.find(id)!=finished.end())
		return finished[id];
	}
///////////////////////////////////////////////////////////////
	void mapFunction::getMap(int fd, int ID){
		pthread_mutex_lock(&mtx);
		map<string,int> jobMap=myMap[ID];
		char buf[MAXLINE];
		for (map<string,int>::iterator it=jobMap.begin(); it!=jobMap.end(); ++it){
		sprintf(buf,"%s*%d\n",it->first.data(),it->second);
  		Rio_writep(fd, buf, strlen(buf));
		}
		pthread_mutex_unlock(&mtx); 
	}

///////////////////////////////////////////////////////////////
void mapFunction::clearMap(){
	myMap.clear();
	finished.clear();
}
///////////////////////////////////////////////////////////////
void mapFunction::printMap(){
	for (std::map<int, map<string,int> >::iterator it=myMap.begin(); it!=myMap.end(); ++it){
		printf("mapWorker's job id is %d\n", it->first);
		for (std::map<string,int>::iterator init=it->second.begin(); init!=it->second.end(); ++init){
				printf("%s: %d\n", init->first.data(), init->second);
			}
	}
}
