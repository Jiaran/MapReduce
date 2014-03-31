#include "cv.h"
#include <stdio.h>
#include <string>

//using namespace std;

const int circleInt= 10;
const int squareInt= 30;
const int triangleInt= 50;



void createPic(int count);

int main(int argc, char** argv){
  
 
  for(int i=0;i<3;i++){
    createPic(i+1);
  }
  
  return 0;
}


void createPic(int count){
  Mat image = Mat::zeros(WIDE,HEIGHT,CV_8UC1);
  int w = rand()%4+1;
  int h = rand()%4+1;
  bool onecircle = false;
  
  for(int i=1;i<=w;i++){
    for(int j=1;j<=h;j++){
      int shape = getRandomShape(circleInt,squareInt, triangleInt, 100);
      switch(shape){
      case CIRCLE:
	if(!onecircle){
	  onecircle=true;
	  drawCircle(w,h,i,j,image);
	
	}
	break;
      case SQUARE :
	drawSquare(w,h,i,j,image);
	break;
      case TRIANGLE:
	drawTriangle(w,h,i,j,image);
	break;
      case RANDOM:
	drawRandom(w,h,i,j,image);
      }
    }
  }
  char name[20];
  sprintf(name,"%d.jpg",count);
  
  printf("%s\n",name);
  imwrite(name,image);
  

}
