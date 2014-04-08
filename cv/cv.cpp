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

void drawSquare(int mx, int my, int x, int y , Mat img){
	int wide = WIDE/mx;
  	int height = HEIGHT/my;
	int thickness = -1;
  	int lineType = 8;
	Point center=getCenter(mx,my,x,y);
	Point corner=Point(center.x-(wide/2-5),center.y-(height/2-5));
	Point oppsite=Point(corner.x+(wide-10),corner.y+(height-10));
	rectangle(img,corner, oppsite, Scalar( 255, 255, 255 ),
	  thickness,
	  lineType);
}

void drawTriangle(int mx, int my, int x, int y, Mat img){
	int thickness = 1;
  	int lineType = 8;
	int wide = WIDE/mx;
  	int height = HEIGHT/my;
	Point center=getCenter(mx,my,x,y);

  /** Create some points */
  Point rook_points[1][3];
  rook_points[0][0] = Point( center.x, center.y-(height/2-5) );
  rook_points[0][1] = Point( center.x-(wide/2-5), center.y+(height/2-5) );
  rook_points[0][2] = Point( center.x+(wide/2-5), center.y+(height/2-5) );

  const Point* ppt[1] = { rook_points[0] };
  int npt[] = { 3 };

  fillPoly( img,
            ppt,
            npt,
            thickness,
            Scalar( 255, 255, 255 ),
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

int circleDetect(Mat image1){
	IplImage* img = new IplImage(image1);
        IplImage* gray = cvCreateImage( cvGetSize(img), img->depth, 1);
        CvMemStorage* storage = cvCreateMemStorage(0);
        cvCvtColor( img, gray, CV_RGB2GRAY );
        cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 );
        CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
        if(circles->total==0) return 0;
	else return 1;
}
