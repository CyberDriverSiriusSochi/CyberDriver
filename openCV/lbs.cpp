#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cmath>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;


struct hist
{
	int values[256];
};
struct mine
{
	int value;
	int x;
	int y;
};

vector <mine> Min;
hist histogram[27][15];
hist histogram1;
int res;
int square_size = 24;

int find_data(int x, int y, int width)
{
	int result = (y-1)*width + x-1;
	return result;
}
void bin(Mat image, Mat new_image, int flag)
{
	unsigned char* data = (unsigned char*)image.data;
	unsigned char* new_data = (unsigned char*)new_image.data;
	int w = image.cols;
	int n = new_image.cols;
	int height = image.rows;

	for(int j = 2; j < height-1; j++)
	{
		for(int i = 2; i < w; i++)
		{
			if(data[find_data(i, j, w)] < data[find_data(i, j-1, w)]) new_data[find_data(i-1, j-1, n)] +=1;
			if(data[find_data(i, j, w)] < data[find_data(i+1, j-1, w)]) new_data[find_data(i-1, j-1, n)] +=2;
			if(data[find_data(i, j, w)] < data[find_data(i+1, j, w)]) new_data[find_data(i-1, j-1, n)] +=4;
			if(data[find_data(i, j, w)] < data[find_data(i+1, j+1, w)]) new_data[find_data(i-1, j-1, n)] +=8;
			if(data[find_data(i, j, w)] < data[find_data(i, j+1, w)]) new_data[find_data(i-1, j-1, n)] +=16;
			if(data[find_data(i, j, w)] < data[find_data(i-1, j+1, w)]) new_data[find_data(i-1, j-1, n)] +=32;
			if(data[find_data(i, j, w)] < data[find_data(i-1, j, w)]) new_data[find_data(i-1, j-1, n)] +=64;
			if(data[find_data(i, j, w)] < data[find_data(i-1, j-1, w)]) new_data[find_data(i-1, j-1, n)] +=128;

			if(flag ==1) histogram[(i-2)/square_size][(j-2)/square_size].values[new_data[find_data(i-1, j-1, n)]]++; 
			else histogram1.values[new_data[find_data(i-1, j-1, n)]]++;
		}
	}
}

bool mat_less (const mine & m1, const mine & m2)
{
	return (m1.value < m2.value);
}

void choose()
{
	for(int y = 0; y < 14; y++)
	{
		for(int x = 0; x < 26; x++)
		{
			res = 0;

			for(int a = 0; a < 256; a++)
			{
				res += (histogram[x][y].values[a] + histogram[x+1][y].values[a] + histogram[x+1][y+1].values[a] + histogram[x][y+1].values[a] - histogram1.values[a])*(histogram[x][y].values[a] + histogram[x+1][y].values[a] + histogram[x+1][y+1].values[a] + histogram[x][y+1].values[a] - histogram1.values[a]);
			}
			 mine m;
			 m.value = res;
			 m.x = x;
			 m.y = y;
			Min.push_back(m);
		}
	}

}

int main()
{
	Min.clear();
	Mat image, mask;
	image = imread("resistors.png", CV_LOAD_IMAGE_GRAYSCALE);
	mask = imread("right_resistor.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat new_image(image.rows-2, image.cols-2, CV_8UC1), new_mask(mask.rows-2, mask.cols-2, CV_8UC1);
	bin(image, new_image, 1);
	bin(mask, new_mask, 2);
	choose();
	sort(Min.begin(), Min.end(), mat_less);
	namedWindow("result");
	for(int i = 0; i < 7; i++)
	rectangle(image, cv::Point(Min[i].x*square_size, Min[i].y*square_size), cv::Point((Min[i].x+2)*square_size, (Min[i].y+2)*24), cv::Scalar(0));
	imshow("result", image);
	waitKey(0);


}