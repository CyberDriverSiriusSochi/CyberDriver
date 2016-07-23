#include "stdafx.h"
#include "head.h"
#define color_depth 64

void ctv_thresholds(Mat &image, int level_num)
{
	unsigned char* data = (unsigned char*)image.data;
	level_num= 256/level_num;
	for(int i=0; i<image.cols*image.rows; ++i)
		data[i]=(data[i]/level_num)*level_num;
}

int main()
{
        VideoCapture capture(1); 
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);//1280); //640
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 180);//960); //360
		namedWindow("result");
        Mat frame, mm, a;
		vector <Mat> mask;
		mask.push_back(imread("Poligon0.png", CV_LOAD_IMAGE_GRAYSCALE));
		mask.push_back(imread("Poligon1.png", CV_LOAD_IMAGE_GRAYSCALE));
		mask.push_back(imread("Poligon2.png", CV_LOAD_IMAGE_GRAYSCALE));

			//GaussianBlur( mask[0], mask[0], Size(5,5), 1.0);
			//ctv_thresholds(mask[0], color_depth);
		int j = 0;
			for(int i = mask[0].cols; i > 20; i-=7)
				{
					resize(mask[0], a, Size(i, i), 0, 0, INTER_LINEAR );
					mask.push_back(a);
					j++;
				}
		
        while(true)
		{
                capture.retrieve(frame);
				cvtColor(frame, mm, CV_RGB2GRAY);
				//GaussianBlur( mm, mm, Size(5,5), 1.0);
                //ctv_thresholds(mm, color_depth);
				for(int i = 0; i < j; i++)
				{
					init(mm, mask[i], "stop");
				}
                imshow("result", Result(mm, 1)); //цифра - это кол-во объектов
				waitKey(1);       
        }
        return 0;
}