#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cstdio>
using namespace cv;
int main()
{
	Mat image, result;
	Size size(200,100);
	double fx = 0, fy = 0;
	image = imread("resistors.png", CV_LOAD_IMAGE_GRAYSCALE);
	int w = image.cols;
	int h = image.rows;
	namedWindow("Window1");
	imshow("Window1", image);
	waitKey(1);

	resize(image, result, size,fx=0, fy, INTER_CUBIC  );
	//Size size1(640,360);
	//resize(result, result, size1,fx=0, fy, INTER_CUBIC  );
	namedWindow("Window");
	imshow("Window", result);
	waitKey(0);
	return 0;
}