#include "stdafx.h"
#include "head.h"

void ctv_thresholds(Mat &image, int level_num)
{
	unsigned char* data = (unsigned char*)image.data;
	level_num= 256/level_num;
	for(int i=0; i<image.cols*image.rows; ++i)
		data[i]=(data[i]/level_num)*level_num;
}

int main()
{
	Mat image = imread("resistors.png", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("a");
	ctv_thresholds(image, 16);
	imshow("a", image);
	waitKey(0);
	return 0;
}