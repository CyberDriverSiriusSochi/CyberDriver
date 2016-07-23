#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
using namespace cv;

int main()
{
        // получаем любую подключённую камеру
        VideoCapture capture(0); //cvCaptureFromCAM( 0 );
      //  assert( capture );

        capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);//1280); 
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 180);//960); 

        // узнаем ширину и высоту кадра
		double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        printf("[i] %.0f x %.0f\n", width, height );

        cv::Mat frame;

        namedWindow("capture", CV_WINDOW_AUTOSIZE);

        printf("[i] press Enter for capture image and Esc for quit!\n\n");

        int counter=0;
        char filename[512];
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
                else if(c == 13) { // Enter
                        // сохраняем кадр в файл
                        sprintf(filename, "Poligon2.png", counter);
                        printf("[i] capture... %s\n", filename);
                        imwrite(filename, mm);
                        counter++;
                }
        }
        // освобождаем ресурсы
        capture.release();
        destroyWindow("capture");
        return 0;
}