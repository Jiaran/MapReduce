#ifndef CV_H
#define CV_H

#include "opencv/cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

#define WIDE 400
#define HEIGHT 400
using namespace cv;
enum {CIRCLE, SQUARE, TRIANGLE,RANDOM};
void drawCircle(int mx, int my, int x, int y , Mat img);
Point getCenter(int mx, int my, int x, int y);
void drawTriangle(int mx, int my, int x, int y, Mat img);
void drawRandom(int mx, int my, int x, int y , Mat img);
void drawSquare(int mx, int my, int x, int y , Mat img);
bool random(double p);
int getRandomShape(int c, int s, int t, int r);
#endif
