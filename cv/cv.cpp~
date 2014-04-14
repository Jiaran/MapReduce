#include "cv.h"
#include <stdlib.h>

void drawSquare (int mx, int my, int x, int y , Mat img ){
}

void drawTriangle (int mx, int my, int x, int y , Mat img ){
}

void drawCircle(int mx, int my, int x, int y , Mat img ){
  int thickness = -1;
  int lineType = 8;
  Point center = getCenter(mx,my,x,y);
  int r = min(WIDE/mx,HEIGHT/my)/2-1;
  
  circle( img,
	  center,
	  r,
	  Scalar( 255, 0, 0 ),
	  thickness,
	  lineType );  

}

Point getCenter(int mx, int my, int x, int y ){
  int wide = WIDE/mx;
  int height = HEIGHT/my;
  Point ans = Point(wide/2, height/2);
  ans.x+= wide* (x-1);
  ans.y+= height*(y-1);
  return ans;
}

void drawRandom(int mx, int my, int x, int y , Mat img){
  Point c = getCenter(mx,my,x,y);
  int wide = WIDE/mx;
  int height = HEIGHT/my;
  for(int i= c.x-wide/2;i<c.x+wide/2;i++){
    if(random(0.5)){
      for(int j=c.y-height/2;j<c.y+height/2;j++){
      
	img.at<uchar>(j, i)=255;
      }
    }
  }
}


bool random(double p){
  double i = rand()%100+1;
  return i<100*p;
}


int getRandomShape(int c, int s,int t ,int r ){
  double i = rand()%100+1;
  if(i<=c){
    return CIRCLE;
  }
  else if(i<=s){
    return SQUARE;
  }
  else if(i<=t){
    return TRIANGLE;
  }
  else{
    return RANDOM;
  }
  

}
