#include "cv.h"
#include "mapImage.cpp"
#include <stdio.h>

int main(int argc, char** argv){
  //char name[]="test window";
  /*Mat image = Mat::zeros(WIDE,HEIGHT,CV_8UC3);
  drawCircle(3,3,2,1,image);
	drawSquare(3,3,1,1,image);
	drawTriangle(3,3,3,1,image);
  imshow(name, image);*/
	mapImage a(1,1);
	a.getImage(0);
	map<string,int> result=a.getMap();
	cout<<result["shishi"]<<endl;
  waitKey(0);
  return 0;
}
