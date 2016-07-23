#include "stdafx.h"
#include "head.h"
#define color_depth 64

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
	a.identifier = "RIGHT ";
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
	
    
    return 0;
}
int main()
{
				
        VideoCapture capture(0); 
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);//1280); //640
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 180);//960); //360
		namedWindow("result");
		VideoWriter vw("./result_new.avi",
			           -1,//CV_FOURCC('X', 'V', 'I', 'D'),
					   20, cv::Size(960, 540));
		if(!vw.isOpened()){
		  fprintf(stderr, "wv error\n");
		  return -1;
		}
		
		Mat frame, mm;
		tesserakt();  
        while(true)
		{
                capture.retrieve(frame);
				cvtColor(frame, mm, CV_RGB2GRAY);
				//GaussianBlur( mm, mm, Size(5,5), 1.0);
				//ctv_thresholds(mm, color_depth);
				for(int i = 0; i < j; i++)
				{
					init(mm, mask[i].image, mask[i].identifier);
				}
				mm = Result(mm, 1, 10);
				resize(mm, frame, Size(960, 540), 0, 0, INTER_LINEAR );
				imshow("result", frame);      //цифра - это кол-во объектов 
				vw.write(frame);
				char c = cvWaitKey(33);
				if ( c != -1) break;
				waitKey(1);
        } 
		
        return 0;

}