#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include "ros/ros.h"
#include "image_proc/ObstacleList.h"
#include "image_proc/Direction.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include"obj_detection.h"
#include"markup_detection.h"

using std::vector;

#define MY_ADDR "10.23.46.164"
#define MY_PORT 4993
using namespace cv;
using namespace std;

extern int num;
extern int r1;

double course(int x, int sc, int sr)
{
	double cour;
	cour = 180 * atan((x-sc/2)/(sr*sqrt(3)))/3.1415;
	return cour;
}

int distanse(int s,int t, int h) //H - âûñîòà ýòàëîíà, L - ðàññòîÿíèå äî ýòàëîíà, h - âûñîòà èçîáðàæåíèÿ
{
	double dist;
	dist = s*h*sqrt(3)/t;
	return dist;
}



int main(int argc, char **argv)
{  
  
tesserakt();
cv::namedWindow("object");
// инициализируем узел
  ros::init(argc, argv, "image_proc");
  ros::NodeHandle n;
  // устанавливаем сетевое соединение с источником видеоданных
  int sockfd, out_sockfd, err;
  socklen_t serv_len, cli_len;
  struct sockaddr_in serv_addr, cli_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd==-1){
    fprintf(stderr, "socket() error\n");
    return -1;
  }    
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    fprintf(stderr, "setsockopt() error");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  inet_pton(AF_INET, MY_ADDR, &serv_addr.sin_addr.s_addr);
  serv_addr.sin_port = htons(MY_PORT);
  serv_len = sizeof(serv_addr);
  err = bind(sockfd, (struct sockaddr *) &serv_addr, serv_len);
  if(err!=0){
    fprintf(stderr, "bind() error\n");
    return -1;
  }
  err = listen(sockfd, 5);
  if(err!=0){
    fprintf(stderr, "listen() error\n");
    return -1;
  }
  // готовимся публиковать данные об обнаруженных препятствиях
  ros::Publisher obstacle_pub = n.advertise<image_proc::ObstacleList>("obstacle", 100);
  ros::Publisher direction_pub = n.advertise<image_proc::Direction>("direction", 100);
  ros::Rate loop_rate(5);
  // создаем изображение для обработки
  cv::Mat image(1,1,CV_8UC1);
  while(1){
    // принимаем входящее соединение
    out_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if(out_sockfd<0){
      fprintf(stderr, "accept() error\n");
      return -1;
    }
    while (ros::ok()){
      int buf_len, read_res;
      // читаем число байтов в сжатом изображении
      read_res = read(out_sockfd, &buf_len, sizeof(int));
      if(read_res<0){
        fprintf(stderr, "read() error: byte number\n");
        return -1;
      }else{
        //fprintf(stderr, "byte number = %i\n", buf_len);
      }
      if(buf_len<=0){
        ;//fprintf(stderr, "Wrong image size: %i %i\n", width, height)
      }
      else{
        fprintf(stderr, "byte number = %i\n", buf_len);
        fprintf(stderr, "Image is correct: %i\n", buf_len);
        // подстраиваем размер изображения под считанные значения
        // (на случай изменения разрешения на лету)
        //resize(image, image, cv::Size(width, height));
        resize(image, image, cv::Size(1,buf_len));
        // читаем точки
        int total_read_num = 0;
        unsigned char * buff_start = (unsigned char *)image.data;
        while(total_read_num<buf_len){
          read_res = read(out_sockfd, buff_start, buf_len);
          if(read_res<0){
            fprintf(stderr, "read() error: data\n");
            return -1;
          }else{
            total_read_num += read_res;
            buff_start += read_res;
          }
        }
        fprintf(stderr, "Data accepted\n");
        image = cv::imdecode(image, CV_LOAD_IMAGE_GRAYSCALE);
        fprintf(stderr, "Image decoded; size is %i x %i\n", image.cols, image.rows);
        char Y = 'Y';
        int wr_res = write(out_sockfd, &Y, sizeof(char));
        if(wr_res<0){
          fprintf(stderr, "write() error\n");
          return -1;
        }
        
        
        fprintf(stderr, "image 1");
        //cv::namedWindow("object");
        imshow("object", find_obj(image));
	
        fprintf(stderr, "image 2");
        int res =  vse2(image);
        fprintf(stderr, "image 3");



 

        cv::namedWindow("data");
        imshow("data", image);
        cv::waitKey(100);
        const int h=8;
//if (distance(image.rows, mask.rows,h)<50){
//pov=0;
     extern bool flag_fo;

//}
        // обрабатываем изображение
        image_proc::ObstacleList obstacle_msg;
        image_proc::Direction direction_msg;
        //direction_msg.left_distance=3*(r1-20);
        //direction_msg.right_distance=3*(res-20);
        if(!flag_fo){
          direction_msg.left_distance= 130 + 0.6*(r1-res);
          direction_msg.right_distance= 130 - 0.6*(r1-res);
        }else{
          direction_msg.left_distance= 50+ 1.3*(r1-res);
          direction_msg.right_distance= 10 - 1.3*(r1-res);
        }
        direction_msg.dir = 0;

        
        obstacle_msg.distance.resize(0);
        obstacle_msg.direction.resize(0);
        //msg.distance[0]=distanse(image.rows, mask.rows,h);
        //msg.direction[0] = course(mine[0].x, image.cols, image.rows);

//////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////        msg.perehod(perehod);

        obstacle_pub.publish(obstacle_msg);
        direction_pub.publish(direction_msg);
        fprintf(stderr, "Completed!!!\n");
        ros::spinOnce();
        loop_rate.sleep();
      }
    }
  }
  return 0; 
}

