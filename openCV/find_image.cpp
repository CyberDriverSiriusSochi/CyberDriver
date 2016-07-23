#include "stdafx.h"
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
cv::Mat corr(cv::Mat s, cv::Mat t, cv::Mat final);

int main(){
    cv::Mat image, mask1, mask2, final;
    image = cv::imread("resistors.png", CV_LOAD_IMAGE_GRAYSCALE); 
    mask1 = cv::imread("left_resistor.png", CV_LOAD_IMAGE_GRAYSCALE);
    mask2 = cv::imread("right_resistor.png", CV_LOAD_IMAGE_GRAYSCALE);
	final = image;
	
	
	cv::namedWindow("First");
	//cv::imshow( "First", corr(image, mask1, final));
	//cv::waitKey(0);
	cv::imwrite("result.png", corr(image, mask2, final));
	cv::waitKey(0);
    return 0;
}


cv::Mat corr(cv::Mat sourse, cv::Mat mask, cv::Mat final)
{
	 double corr;
	 int mask_now;
	 int sourse_now;
	 double sum_image = 0, sum_mask = 0;
	 unsigned char* s_data = (unsigned char*)sourse.data;
	 unsigned char* m_data = (unsigned char*)mask.data;
	 cv::Mat result(sourse.rows-mask.rows+1, sourse.cols-mask.cols+1, CV_8UC1);
	 unsigned char* r_data = (unsigned char*)result.data;

	 for(int i = 0; i < result.rows; i++)
	 {
		for(int j = 0; j < result.cols; j++)
		{
			// считаем корреляцию, которая получится, если приложить верхний левый угол шаблона к точке изображения (i,j)
			corr = 0;
			sum_image = 0;
			sum_mask = 0;
		    for(int x = 0; x < mask.cols; x++)
			{
				for(int y = 0; y < mask.rows; y++)
				 {
					mask_now = y*mask.cols + x;
					sourse_now = (y+i)*sourse.cols + (x+j);

					corr += m_data[mask_now] * s_data[sourse_now];
					sum_image += s_data[sourse_now] * s_data[sourse_now];
					sum_mask += m_data[mask_now] * m_data[mask_now];
				}
			}
			if(sum_image==0 || sum_mask==0)
            r_data[i*result.cols + j] = 0;
			else
			{
				r_data[i*result.cols + j] = 255 * corr / sqrt(sum_image) / sqrt(sum_mask);
			}
			// напечатаем сообщение, если объект очень похож на шаблон
			if(r_data[i*result.cols + j]>252)
			{
              printf("Object found in (%i, %i)%c", i+mask.rows/2,j+mask.cols/2, '\n');
			  rectangle(final, cv::Point(j,i), cv::Point(j+mask.cols, i+mask.rows), cv::Scalar(0));
			}
			
		}

	}
	 printf("%c", '!');
  return final;
}
