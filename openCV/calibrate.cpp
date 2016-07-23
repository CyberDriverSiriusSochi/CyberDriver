#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
using namespace cv;

int main()
{
        VideoCapture capture(0);
        assert( capture );

        capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

        // узнаем ширину и высоту кадра
		double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        printf("[i] %.0f x %.0f\n", width, height );

        cv::Mat frame;

        namedWindow("capture", CV_WINDOW_AUTOSIZE);

        printf("[i] press Enter for capture image and Esc for quit!\n\n");
		cv::Mat mm;
        while(true){
                // получаем кадр
                capture.retrieve(frame);

				cvtColor(frame, mm, CV_RGB2GRAY);
                // показываем
               imshow("capture", mm);
        
                char c = cvWaitKey(33);
                if (c == 27) { // нажата ESC
                        break;
                }
        }
		unsigned char* data = (unsigned char*)mm.data;
		for(int i = 0; i < width*height; i++)
		{
			if(data[i] > 130) data[i] = 255;
			else data[i] = 0;
		}
		imshow("capture", mm);
		waitKey(0);
        // освобождаем ресурсы
        capture.release();
        destroyWindow("capture");
        return 0;
}