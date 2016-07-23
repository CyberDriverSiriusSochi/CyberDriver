#include "opencv/highgui.h"
#include"obj_detection.h"

#include <opencv/cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#define ORIGINAL true
#define MASK false

using namespace cv;
using namespace std;
struct hist
{
	int values[256];
	int x;
	int y;
	int coof;
	int size;
	string name;
	hist()
	{
		for(int i = 0; i < 256; i++) values[i] = 0;
		coof = 1000000000;
	}
};

int mask_hist[256] = {};
hist **image_hists;
vector <hist> result;
int quad_size = 48; //ïî óìîë÷àíèþ
int imax, jmax;

struct images
{
	Mat Image, new_image;
	int lenght;
	int height;
	unsigned char* data;
	unsigned char* new_data;
	bool cr_mode;

	images(Mat a, bool mode) //if mode == true that will be image mode else mask mode
	{
		Image = a;
		lenght = a.cols;
		height = a.rows;
		data = (unsigned char*)a.data;
		new_image.create(height - 2, lenght -2, CV_8UC1);	
		new_data = (unsigned char*)new_image.data;
		if(mode)
		{
			image_hists = new hist *[(lenght-2)/quad_size+1];
			for(int i = 0; i < (lenght-2)/quad_size+1; i++) image_hists[i] = new hist[(height-2)/quad_size+1];
		}
		else 
quad_size = (lenght-2)/2;
		cr_mode = mode;
		bin(mode);
	}
	~images(){
		if(cr_mode){
		  for(int i = 0; i < (lenght-2)/quad_size+1; i++)
		    delete [] image_hists[i];
	      delete [] image_hists;
		}
	}

	int find_data(int x, int y, int width)
	{
		int resulte = (y-1)*width + x-1;
		return resulte;
	}

	void bin(bool mode) //if mode == true that will be image mode else mask mode
	{
		int a1, a2;
		for(int j = 2; j < height-1; j++)
		{
			for(int i = 2; i < lenght; i++)
			{
				a1 = find_data(i, j, lenght);
				a2 = find_data(i-1, j-1, lenght-2);
				new_data[a2] = 0;
				if(data[a1] < data[find_data(i, j-1, lenght)]) new_data[a2] +=1;
				if(data[a1] < data[find_data(i+1, j-1, lenght)]) new_data[a2] +=2;
				if(data[a1] < data[find_data(i+1, j, lenght)]) new_data[a2] +=4;
				if(data[a1] < data[find_data(i+1, j+1, lenght)]) new_data[a2] +=8;
				if(data[a1] < data[find_data(i, j+1, lenght)]) new_data[a2] +=16;
				if(data[a1] < data[find_data(i-1, j+1, lenght)]) new_data[a2] +=32;
				if(data[a1] < data[find_data(i-1, j, lenght)]) new_data[a2] +=64;
				if(data[a1] < data[find_data(i-1, j-1, lenght)]) new_data[a2] +=128;
				if(mode)
				{
					image_hists[(i-2)/quad_size][(j-2)/quad_size].values[new_data[a2]]++;
					image_hists[(i-2)/quad_size][(j-2)/quad_size].x = i;
					image_hists[(i-2)/quad_size][(j-2)/quad_size].y = j;
					imax = 	(i-2)/quad_size;
					jmax = (j-2)/quad_size;	
				}
				else mask_hist[new_data[a2]]++;
			}
		}
		
	}
};





bool mat_less (const hist & m1, const hist & m2)
{
	return (m1.coof < m2.coof);
}

void choose(string id)  //íîìåð ìàñêè 0 - n
{
	hist res;
	int r;
	for(int j = 0; j < jmax; j++)
	{
		for(int i = 0; i < imax; i++)
		{
			res = image_hists[i][j];
			res.coof = 0;
			for(int g = 0; g < 256; g++)
			{
				r = (image_hists[i][j].values[g]+image_hists[i+1][j].values[g]+image_hists[i+1][j+1].values[g]+image_hists[i][j+1].values[g] - mask_hist[g]);
				image_hists[i][j].values[g] = 0;
				image_hists[i][j].x = 0;
				image_hists[i][j].y = 0;
				res.coof += r*r;
			}
			res.coof = res.coof/quad_size/quad_size;
			res.size = quad_size;
			res.name = id;
			result.push_back(res);
		}
	}
}
//int distanse(int H, int h) //H - âûñîòà ýòàëîíà, L - ðàññòîÿíèå äî ýòàëîíà, h - âûñîòà èçîáðàæåíèÿ
//{
//	double dist;
//	dist = H*focus/h;
//	return dist;
//}

void init(Mat original, Mat maske, string id)
{
	images mask(maske, MASK);
	images first(original, ORIGINAL);
	choose(id);
	imax = 0;
	jmax = 0;
	for(int i = 0; i < 256; i++) mask_hist[i] = 0;

}
Mat Result(Mat first, int counter, int coef)
{
        fprintf(stderr, "Result processing started\n");
	sort(result.begin(), result.end(), mat_less);
        if(counter>=result.size())
          counter=result.size();
	for(int i = 0; i < counter; i++)
	{
                if(i>=result.size())
                  fprintf(stderr, "Result size error\n");
		//cout<<result[i].coof<<endl;
		if(result[i].coof < coef) 
		{
			cout<<result[i].name<<endl;
			rectangle(first, Point(result[i].x-result[i].size,result[i].y-result[i].size), Point(result[i].x+result[i].size,result[i].y+result[i].size), Scalar(0));
		}
	}
	result.clear();
	fprintf(stderr, "Result processing ended\n");
	return first;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

struct object
{
	Mat image;
	string identifier; 
};
void ctv_thresholds(Mat &image, int level_num)
{
	unsigned char* data = (unsigned char*)image.data;
	level_num= 256/level_num;
	for(int i=0; i<image.cols*image.rows; ++i)
		data[i]=(data[i]/level_num)*level_num;
}
int j = 0;
vector <object> mask;

int tesserakt()
{
	object a;

	a.image = imread("Poligon0.png", CV_LOAD_IMAGE_GRAYSCALE);
	//GaussianBlur( a.image, a.image, Size(5,5), 1.0);
	//ctv_thresholds(a.image, color_depth);
	a.identifier = "STOP";
	mask.push_back(a);

	//a.image = imread("Poligon1.png", CV_LOAD_IMAGE_GRAYSCALE);
	//a.identifier = "left";
	//mask.push_back(a);

	a.image = imread("Poligon2.png", CV_LOAD_IMAGE_GRAYSCALE);
	//GaussianBlur( a.image, a.image, Size(5,5), 1.0);
	//ctv_thresholds(a.image, color_depth);
	a.identifier = "RIGHT";
	mask.push_back(a);
	
	
	//GaussianBlur( mask[0], mask[0], Size(5,5), 1.0);
	//ctv_thresholds(mask[0], color_depth);
	for(int g = 0; g < 2; g++)
	{
		for(int i = mask[g].image.cols; i > 15; i-=5)
			{
				resize(mask[g].image, a.image, Size(i, i), 0, 0, INTER_LINEAR );
				a.identifier = mask[g].identifier;
				mask.push_back(a);
				j++;
			}
	}
	//GaussianBlur(frame, frame, Size(5,5), 1.0);
   // ctv_thresholds(frame, color_depth);
	
    printf("tesserakt done\n");
    return 0;
}
cv::Mat find_obj(cv::Mat frame)
{
				
	Mat  mm;
	//cvtColor(frame, mm, CV_RGB2GRAY);
	//GaussianBlur( mm, mm, Size(5,5), 1.0);
	//ctv_thresholds(mm, color_depth);
	for(int i = 0; i < j; i++)
	{
		init(frame, mask[i].image, mask[i].identifier);
	}
	printf("object started\n");
	frame = Result(frame, 1, 20);
	resize(frame, mm, Size(960, 540), 0, 0, INTER_LINEAR ); 
	printf("object ended\n");        
	return mm;

}
