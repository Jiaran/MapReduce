#include "cv.h"



void drawCircle(int mx, int my, int x, int y , Mat img ){
  int thickness = -1;
  int lineType = 8;
  Point center = getCenter(mx,my,x,y);
  int r = min(WIDE/mx,HEIGHT/my)/2-1;
  
  circle( img,
	  center,
	  r,
	  Scalar( 255, 255, 255 ),
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
