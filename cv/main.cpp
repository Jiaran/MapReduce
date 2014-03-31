#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
  char name[]="test window";
  Mat image = Mat::zeros(WIDE,HEIGHT,CV_8UC3);
  drawCircle(3,3,2,1,image);
  imshow(name, image);
  waitKey(0);
  return 0;
}
