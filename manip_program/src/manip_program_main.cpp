#include<vector>
#include<cmath>
#include<cstdio>
#include<fcntl.h> 
#include<termios.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ros/ros.h"
#include "image_proc/ObstacleList.h"
#include "image_proc/Direction.h"


using std::vector;

struct dangerous_zone{
  bool is_obstacle;
  float left_angle;
  float right_angle;
} dz;

struct safety_zone{
  bool is_obstacle;
  float left_angle;
  float left_dist;
  float right_angle;
  float right_dist;
} sz;

const float sz_width = 0.2;
const float dz_dist = 0.19;
const float sz_dist = 0.40;

float deg2rad(float angle){
  return angle * M_PI / 180;
}

float rad2deg(float angle){
  return angle / M_PI * 180;
}

struct manipulator_state{
  float l[3];
  float angle[3];
  float omega;
};
float dist2line=0;
float ygol=0;
float k,s;

manipulator_state m; 
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int choose_the_direction_dz(dangerous_zone dz,int &n){
  if (dz.is_obstacle = true){    
      if (dz.right_angle>dz.left_angle){
           n=-1;}
      else{n=1;}
  }
  else {n=0;}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int choose_the_direction_sz(safety_zone sz,int &m){
  if (sz.is_obstacle = true){    
      if (sz.right_angle>sz.left_angle){
           m=-1;}
      else{m =1;}
  }
  else {m=0;}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//прямая задача кинематики
void coord(manipulator_state m,
        float &x_end,
        float &y_end,
        float &z_end,
        int &error){
  float x[3];
  float y[3];
  float angle = m.angle[0];
  //ищем координаты суставов в подвижной системе координат
  x[0]=m.l[0]*cos(angle);
  y[0]=m.l[0]*sin(angle);

  for(int i=1; i<3; i = i+1){
    angle = angle + m.angle[i];
    x[i] = x[i-1] + m.l[i]*cos(angle);
    y[i] = y[i-1] + m.l[i]*sin(angle);
  }

  // ищем углы между звеньями и осью OX
  float angle1 = m.angle[0];
  float angle2 = angle1 + m.angle[1];
  float angle3 = angle2 + m.angle[2];

  // проверка на ошибки положения манипулятора
  if (y[0] <= 0 || y[1] <= 0 || y[2]<=0 || m.angle[1]==M_PI || m.angle[2]==M_PI || m.angle[1]==-M_PI || m.angle[2]==-M_PI){
    error = -2;
    return;
  }

  //проверка на пересечение
  bool k=0;
  float xp;
    
    //x[1]=x[2]
  if (fabs(x[1]-x[2])<0.0001){
    xp=x[1]*x[2]/y[0];
    k=1;
    if (xp>=0 && xp<=x[0]){ 
      error = -3;  
      return;}
  } 

    //x[0]=0
  if (fabs(x[0])<0.0001 && k==0){
    k=1;
    if (fabs(y[1]-y[2])<0.0001){
      if (x[1]*x[2]<=0 || y[1]>y[0]){ 
        error = -3;  
        return;
      }
    }
    else{
      xp=x[1]-y[1]*(x[1]-x[2])/(y[1]-y[2]); 
      if (xp>=0 && xp<=x[0]){ 
        error = -3;  
        return;}
    }
  }

    //окончательная формула проверки пересечения
  if (k==0 && fabs(y[0]/x[0]-(y[1]-y[2])/(x[1]-x[2]))>=0.0001){
    xp=(y[1]-x[1]*(y[1]-y[2])/(x[1]-x[2]))/(y[0]/x[0]-(y[1]-y[2])/(x[1]-x[2])) ;
    if (xp>=0 && xp<=x[0] && xp>=x[2]){ 
        error = -3;
        return;}
  }

  // ищем 2 коррдинаты в неподвижной системе координат
  float x0[3];
  for(int i=0;i<3;i++){
  x0[i] = x[i]*cos(m.omega);
  }

  float z[3];
  for (int i=0;i<3;i++){
  z[i] = x[i]*sin(m.omega);
  }
  
  x_end = x0[2];
  y_end = y[2];
  z_end = z[2];
  error = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void anticoord(manipulator_state m,float tox,float toy,float toz,float &alpha,float  &beta,float &gamma,float &amega){
  float a,b,c,anglea,angleb;
  c = sqrt(toy*toy+(sqrt(tox*tox+toz*toz)-m.l[2])*(sqrt(tox*tox+toz*toz)-m.l[2]));
  a = ((m.l[0])*(m.l[0]) - (m.l[1])*(m.l[1]) + c*c) / (2*c);
  b = c-a;
  anglea = asin(a/m.l[0]);
  angleb = asin(fabs(b)/m.l[1]);
  alpha = (deg2rad(90)-anglea)+asin(toy/c);
  beta = -(deg2rad(180)-anglea-angleb);
  gamma = -(deg2rad(90)-(deg2rad(270)-alpha-anglea-angleb));
  amega = atan(toz/tox);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//проверка задачи обратной кинематики
int unit_test_anticoord(float tox, float toy, float toz){
  //тест 3:
  float alpha, beta, gamma, omega,x_end, y_end, z_end;
  manipulator_state m; 
  int error;
  
  m.l[0] = 12;
  m.l[1] = 10;
  m.l[2] = 8;

  m.omega = deg2rad(0);  

  anticoord(m,tox,toy,toz,alpha,beta,gamma,omega);
  m.angle[0]=alpha;
  m.angle[1]=beta;
  m.angle[2]=gamma;
  m.omega=omega;
  coord( m,x_end,y_end,z_end,error);
  if (fabs(x_end-tox)>0.001 || fabs(y_end-toy)>0.001 || fabs(z_end-toz)>0.001 || error!=0){
    printf("wrong solvation" );
    return -1;}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//проверка задачи прямой кинематики
int unit_test_coord(){
  // тест 1: нормальное позиционирование
  // зададим состояние манипулятора 
  manipulator_state m; 

  m.l[0] = 7;
  m.l[1] = 4;
  m.l[2] = 6;

  m.angle[0] = deg2rad(60);
  m.angle[1] = deg2rad(-45);
  m.angle[2] = deg2rad(-90);
  m.omega = deg2rad(0);  

  // решаем прямую задачу
  float x_end, y_end, z_end;
  int error;
  coord(m,x_end, y_end, z_end, error);
  if(error!=0){
    printf("Error in error: %i instead of 0\n", error);
    return -1;
  }
  if(fabs(x_end-8.916617)>0.001){
    printf("Error in x_end: %f instead of %f\n", x_end, 8.916617);
    return -1;
  }
  if(fabs(y_end-1.301900)>0.001){
    printf("Error in y_end: %f instead of %f\n", y_end, 1.301900);
    return -1;
  }
  if(fabs(z_end)>0.001){
    printf("Error in z_end: %f instead of %f\n", z_end, 0.0);
    return -1;
  }
  
  // тест 2. Самопересечение
  m.l[0] = 12;
  m.l[1] = 6;
  m.l[2] = 8;

  m.angle[0] = deg2rad(60);
  m.angle[1] = deg2rad(-133);
  m.angle[2] = deg2rad(-144);
  m.omega = deg2rad(0);  

  // решаем прямую задачу
  coord(m,x_end, y_end, z_end, error);
  if(error!=-3){
    printf("Error in error: %i instead of -3\n", error);
    return -1;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  


void to_polar(float &r, float &dir, float x, float y){
  r = sqrt(x*x + y*y);
  dir = -atan2(y,x);
}
 
void from_polar(float &x, float &y, float r, float dir){
  x = r*cos(dir);
  y = -r*sin(dir);
}

// тест для преобразования координат в полярные и обратно
int test_polar(){
  float x,y,r,dir;
  to_polar(r,dir, 3, 4);
  if(fabs(r-5)>0.001 || fabs(dir+atan2(4,3))>0.001){
    fprintf(stderr, "test_polar test 1 error %f %f\n", r, dir);
    return -1;
  }
  from_polar(x,y,r,dir);
  if(fabs(x-3)>0.001 || fabs(y-4)>0.001){
    fprintf(stderr, "test_polar test 2 error\n");
    return -1;
  }
  return 0;
}

int process_message_about_obstacles(const vector<float>& dists_array,
                                    const vector<float>& dirs_array){
  const float obstacle_size = 0.01; // метров
  unsigned int size = dists_array.size();
  if(dirs_array.size()!=size){
    return -1;
  }
  // обнуляем прошлое состояние поиска
  sz.is_obstacle = false;
  dz.is_obstacle = false;
  // считываем данные из контейнера
  float x, y;
  for(unsigned int i=0; i<size; ++i){
    // считаем декартовы координаты препятствия
    from_polar(x,y,dists_array[i], dirs_array[i]);
    // 1. Если его центр входит в опасную зону 
    if(x<dz_dist && y<sz_width/2 && y>-sz_width/2){
      // вычислим угол, под которым оно будет видно камере
      if(y<0){
        // если препятствие справа от направления машины,
        // берем левый верхний угол как самый левый, 
        // правый нижний как самый правый
        float dir, dist;
        to_polar(dist, dir, x+obstacle_size/2, y+obstacle_size/2);
        // если левый угол левее прошлого крайнего, сохраним его
        if(dz.is_obstacle==false || (dz.left_angle>dir)){
          dz.left_angle  = dir;
        }
        to_polar(dist, dir, x-obstacle_size/2, y-obstacle_size/2);
        // если правый угол правее прошлого крайнего, сохраним его
        if(dz.is_obstacle==false || (dz.right_angle<dir)){
          dz.right_angle  = dir;
        }
      }else{
        // если препятствие слева от направления машины,
        // берем левый нижний угол как самый левый, 
        // правый верхний как самый правый
        float dir, dist;
        to_polar(dist, dir, x-obstacle_size/2, y+obstacle_size/2);
        // если левый угол левее прошлого крайнего, сохраним его
        if(dz.is_obstacle==false || (dz.left_angle>dir)){
          dz.left_angle  = dir;
        }
        to_polar(dist, dir, x+obstacle_size/2, y-obstacle_size/2);
        // если правый угол правее прошлого крайнего, сохраним его
        if(dz.is_obstacle==false || (dz.right_angle<dir)){
          dz.right_angle  = dir;
        }
      }
      // пометим, что препятствие найдено
      dz.is_obstacle = true;
    }else{
      // 2. Если его центр входит в зону безопасности 
      if(x<sz_dist && y<sz_width/2 && y>-sz_width/2){
        // вычислим угол, под которым оно будет видно камере
        if(y<0){
          // если препятствие справа от направления машины,
          // берем левый верхний угол как самый левый, 
          // правый нижний как самый правый  
          float dir, dist;
          to_polar(dist, dir, x+obstacle_size/2, y+obstacle_size/2);
          // если левый угол левее прошлого крайнего, сохраним его
          if(sz.is_obstacle==false || (sz.left_angle>dir)){
            sz.left_angle = dir;
            sz.left_dist = dist;
          }
          to_polar(dist, dir, x-obstacle_size/2, y-obstacle_size/2);
          // если правый угол правее прошлого крайнего, сохраним его
          if(sz.is_obstacle==false || (sz.right_angle<dir)){
            sz.right_angle = dir;
            sz.right_dist = dist;
          }
        }else{
          // если препятствие слева от направления машины,
          // берем левый нижний угол как самый левый, 
          // правый верхний как самый правый
          float dir, dist;
          to_polar(dist, dir, x-obstacle_size/2, y+obstacle_size/2);
          // если левый угол левее прошлого крайнего, сохраним его
          if(sz.is_obstacle==false || (sz.left_angle>dir)){
            sz.left_angle  = dir;
            sz.left_dist = dist;
          }
          to_polar(dist, dir, x+obstacle_size/2, y-obstacle_size/2);
          // если правый угол правее прошлого крайнего, сохраним его
          if(sz.is_obstacle==false || (sz.right_angle<dir)){
            sz.right_angle  = dir;
            sz.right_dist = dist;
          }
        }
        // пометим, что препятствие найдено
        sz.is_obstacle = true;
      }
    }
  }
  return 0;
}

int test_process_message_about_obstacles(){
  sz.is_obstacle = false;
  dz.is_obstacle = false;
  vector<float> dists(4);
  vector<float> dirs(4);
  dists[0] = 0.15;
  dists[1] = 0.27;
  dists[2] = 0.27;
  dists[3] = 0.8;
  dirs[0] = 0.1;
  dirs[1] = -0.3;
  dirs[2] = 0.2;
  dirs[3] = 0;
  int res = process_message_about_obstacles(dists, dirs);
  if(res!=0){
    fprintf(stderr, "test_process_message_about_obstacles test 1 error\n");
    return -1;
  }
  if(dz.is_obstacle==false){
    fprintf(stderr, "test_process_message_about_obstacles test 2 error\n");
    return -1;
  }
  if(sz.is_obstacle==false){
    fprintf(stderr, "test_process_message_about_obstacles test 3 error\n");
    return -1;
  }
  // тесты не закончены; надо проверить значения
  sz.is_obstacle = false;
  dz.is_obstacle = false;
  return 0;
}

// читаем данные о препятствиях и преобразуем их
void obstacleCallback(const image_proc::ObstacleList::ConstPtr & msg){
  int res;
  res = process_message_about_obstacles(msg->distance,
                                         msg->direction);
  if(res==-1)
    fprintf(stderr, "Wrong data format\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float left_distance, right_distance;
int direction;
float tox, toy, toz;


int sockfd;
    char buf[100];
int serverlen;
    struct sockaddr_in serveraddr; 


int USB_manip = -1;
    

void directionCallback(const image_proc::Direction::ConstPtr & msg){
  left_distance = msg->left_distance;
  right_distance = msg->right_distance;
  direction = msg->dir;
  fprintf(stderr, "%i\n", direction);
  if ( left_distance >= right_distance  ){
    //команда для поворота налево в движении
    
    //////////////////////////////
    sprintf(buf, "%i %i\n", int(left_distance+0.5), int(right_distance+0.5));
//    sendto(sockfd, buf, strlen(buf), 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
    //fprintf(stderr, buf);
    
  }
  if ( left_distance < right_distance  ){
    //команда для поворота направо в движении
     sprintf(buf, "%i %i\n", int(left_distance+0.5), int(right_distance+0.5));
//     sendto(sockfd, buf, strlen(buf), 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
     //fprintf(stderr, buf);
    //////////////////////////////
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int open_serial_port(const char *dev_name, int &USB){
  USB = open( dev_name, O_RDWR| O_NOCTTY );
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);

  if ( tcgetattr ( USB, &tty ) != 0 ) {
    fprintf(stderr, "Attributes getter error\n");
    return -1;
  }
  tty_old = tty;

  /* скорость передачи */
  cfsetospeed (&tty, (speed_t)B9600);
  cfsetispeed (&tty, (speed_t)B9600);

  /* атрибуты передачи */
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;

  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

  cfmakeraw(&tty);

  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
    fprintf(stderr, "Error in attribute setter\n");
    return -1;
  }
  return 0;
}

int write_in_serial_port(int USB, void *data, int num_in_bytes){
  int n_written = write( USB, data, num_in_bytes);
  if(n_written==num_in_bytes)
    return 0;
  else
    return -1;
}

int USB = -1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void timerCallback(const ros::TimerEvent& e){
  write_in_serial_port(USB_manip, &direction, sizeof(int));

  /*
  // код, который по данным о препятствиях должен рассчитать,
  // какие команды надо подавать манипулятору
  int n;
  choose_the_direction_dz(dz,n);
  int t;
  choose_the_direction_sz(sz,t);
  if  (n=-1){
  //команда для поворота налево на месте
  //////////////////////////////
  ygol=ygol-k;
  } 
  if  (n=1){
  //команда для поворота направо на месте
  //////////////////////////////
  ygol=ygol+k;
  }    
  if  (n=0){
    if (ygol>0){
         //команда для поворота налево в движении
         //////////////////////////////
         dist2line=dist2line+sin(ygol);
         ygol=ygol-s;
         }
    if (ygol<0){
         //команда для поворота направо в движении
         //////////////////////////////
         dist2line=dist2line-sin(ygol);
         ygol=ygol+s;
         }
    if (ygol=0){
         if (dist2line>0){
               //команда для поворота налево в движении
               //////////////////////////////
               dist2line=dist2line-sin(ygol);
               ygol=ygol-s;
               }
         if (dist2line<0){
               //команда для поворота направо в движении
               //////////////////////////////
               dist2line=dist2line+sin(ygol);
               ygol=ygol+s;
               }
         if (dist2line=0){
                 if  (t=-1){
                       //команда для поворота налево в движении
                       //////////////////////////////
                       ygol=ygol-s;
                 } 
                 if  (t=1){
                       //команда для поворота направо в движении
                       //////////////////////////////
                       ygol=ygol+s;
                 }    
                 if  (t=0){
                       //команда для прямого движения
                       //////////////////////////////
                 } 
               }

    }
  } 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // задаем координаты конечной точки манипулятора
  int error;
  error = open_serial_port("/dev/ttyUSB0", USB);
  if(error!=0){
    fprintf(stderr, "Error %i while connecting to manipulator\n", error);
  }
  //тест 1-2
  int unit_test_coord();
  //тест 3
  unit_test_anticoord(tox,toy,toz);

  // решаем обратную задачу
  float alpha,beta,gamma,amega;
  anticoord(m,tox,toy,toz,alpha,beta,gamma,amega);

  // если произошла ошибка, выводим ее номер и выходим
  if(error!=0){
    printf("Error %i while calculating coord\n", error);
  } 
  if(error==-3){
    printf("manip has crossed itself\n");
    return ;
  }
  if(error==-2){
    printf("manip's location is wrong\n");
    return ;
  }
    // выводим ответ
  float data[4];
  data[0]=rad2deg(amega);
  data[1]=rad2deg(alpha);
  data[2]=rad2deg(beta);
  data[3]=rad2deg(gamma);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  error = write_in_serial_port(USB, data, 4*sizeof(float));
  if(error!=0){
    fprintf(stderr, "Error %i while sending data to manipulator\n", error);
  }*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv){


    char hostname[] = "10.23.47.251";
    int port = 4999;


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    inet_aton(hostname, &serveraddr.sin_addr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    
    int error;
    error = open_serial_port("/dev/ttyUSB0", USB_manip);



  float alpha,beta,gamma,omega;  
  if(unit_test_coord()!=0){
    printf("Error in unit test\n");
    return -1;
  }

  // зададим состояние манипулятора 
  
  m.l[0] = 12;
  m.l[1] = 10;
  m.l[2] = 10;

 
   //write_in_serial_port(int USB, void *data, int num_in_bytes)


  // встроенные тесты
  if(test_polar()!=0){
    return -1;
  }
  if(test_process_message_about_obstacles()!=0){
    return -1;
  }
  // инициализируем узел
  ros::init(argc, argv, "manip_program");
  ros::NodeHandle n;
  // подписываемся на события 
  ros::Subscriber sub_obstacle = n.subscribe("obstacle", 100, obstacleCallback);
  ros::Subscriber sub_direction = n.subscribe("direction", 100, directionCallback);
  // создаем таймер, чтобы отправлять данные контроллеру манипулятора
  // ros::Timer timer = n.createTimer(ros::Duration(0.1), timerCallback);
  // обрабатываем сообщения до сигнала завершения программы 
  ros::spin();
  return 0; 
}
