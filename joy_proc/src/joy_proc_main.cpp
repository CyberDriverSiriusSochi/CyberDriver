#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

#include "ros/ros.h"
#include "sensor_msgs/Joy.h"

#define SRV_IP "127.0.0.1"

char buf[BUFLEN];
int s;
struct sockaddr_in si_other;
  

// читаем данные с джойстика и отправляем их на плату
void joyCallback(const sensor_msgs::Joy::ConstPtr & msg){
  // считываем нужные данные из сообщения джойстика
  float dx = msg->axes[0];
  float dy = msg->axes[1];
  // записываем их в буфер в виде текста
  sprintf(buf, "%f %f", dx, dy);
  // отправляем их по сети на плату
  if(sendto(s, buf, BUFLEN, 0, (sockaddr*)&si_other, sizeof(si_other))==-1){
    fprintf(stderr, "Error while data sending\n");
  }
}

int main(int argc, char ** argv){
  // инициализируем узел
  ros::init(argc, argv, "joy_proc");
  ros::NodeHandle n;
  // устанавливаем связь с платой
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    fprintf(stderr, "Error while socket creation\n");
    return -1;
  }

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    return -1;
  }

  // подписываемся на события джойстика
  ros::Subscriber sub = n.subscribe("Joy", 100, joyCallback);
  // обрабатываем сообщения до сигнала завершения программы 
  ros::spin();
  return 0; 
}
