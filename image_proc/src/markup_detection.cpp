#include"markup_detection.h"

#define num_lines 4

bool perehod=0;
int res1;

int r1=0;

bool flag_fo = false;
 
int vse2(cv::Mat& image){
  unsigned char* image_data = (unsigned char*)image.data;//получить доступ к изображению
  // найти разметку везде, начав снизу.
  const float lines_rel_to_find[num_lines] = {0.9, 0.88, 0.86, 0.84};
  int lines_to_find[num_lines];
  for(int i=0;i<num_lines; ++i){
    lines_to_find[i] = int(lines_rel_to_find[i] * image.rows);
  }
  
  int threshold = 100;
  for(int i = 0; i<num_lines; ++i){
    // ищем правую
    bool found_r = false;
    bool found_l = false;
    int rborder = -1, lborder = -1;
    int x;
    flag_fo = false;
    for(x = image.cols/2; x<image.cols; ++x){
      if(image_data[lines_to_find[i]*image.cols+x]<threshold){
        rborder = x - image.cols/2;
        found_r = true;
        break;
      }else{
        image_data[lines_to_find[i]*image.cols+x] = 0;
      }
    }
    for(x = image.cols/2-1; x>=0; --x){
      if(image_data[lines_to_find[i]*image.cols+x]<threshold){
        lborder = image.cols/2 - x;
        found_l = true;
        break;
      }else{
        image_data[lines_to_find[i]*image.cols+x] = 0;
      }
    }
    for(int y=lines_to_find[i]-1;y>3*image.rows/4;--y){
      if(image_data[y*image.cols+image.cols/2]<threshold){
        flag_fo = true;
        break;
      }
    }
    if(found_r && found_l){
      r1 = rborder;
      return lborder;
    }
    if(!found_r && found_l){
      r1 = 0.7*image.cols;
      return lborder;
    }
    if(found_r && !found_l){
      r1 = rborder;
      return 0.7*image.cols;
    }
  }
      r1 = 0.7*image.cols;
      return 0.7*image.cols;

  
}

int vse(cv::Mat& image)
{
  int sch=0;//обнуляем счетчик
  int sr=0;//среднее значение клеток
  int r=0;//расстояние в одной из двух частей и.
  int raz=0;//обнуляем разницу
  int s=0;//
  unsigned char* image_data = (unsigned char*)image.data;//получить доступ к изображению
  int kl=image.cols*image.rows;//количество клеток в изображении
  for(int y_s=0; y_s<image.cols; y_s++)
  {
    for(int x_s=0; x_s<image.rows; x_s++)//движение по изображению
    {
      sch+=image_data[x_s*image.cols+y_s];//счетчик
    }
  }
  fprintf(stderr,"1\n");
  sr=sch/kl;//считаем среднее всех клеток по яркости
  
  for(int y=0; y<(image.cols/2); y++)//движение по половине изображения
  {
    for(int x=0; x<=image.rows; x++)
    {
      if(x==image.rows*6/12)//
      {
        if(image_data[x*image.cols+y]+20<sr)//если значение яркости клетки +20 меньше средней яркости изображения
        {
          image_data[x*image.cols+y]=255;//сделать клетку белой
          r=(image.cols/2)-y;//расстояние от центра до клетки
          s++;//
        } 
        else //иначе
        {
          if(x==image.rows*8/12)//
          {
            if(image_data[x*image.cols+y]+20<sr)//если значение яркости клетки +20 меньше средней яркости изображения
            {
              image_data[x*image.cols+y]=255;//сделать клетку белой
              r=(image.cols/2)-y;//расстояние от центра до клетки
              s++;//
            }
            else //иначе
            {
              if(x==image.rows*10/12)//
              {
                if(image_data[x*image.cols+y]+20<sr)//если значение яркости клетки +20 меньше средней яркости изображения
                {
                  image_data[x*image.cols+y]=255;//сделать клетку белой
                  r=(image.cols/2)-y;//расстояние от центра до клетки
                  s++;//
                }
                else //иначе
                {
                  fprintf( stderr, "no_l");
                }
              }   
            }
          }
        }
      }
    }
  }
  fprintf(stderr, "%i\n", r);
  for(int y=(image.cols/2); y<image.cols; y++)//движение по половине изображения
  {
    for(int x=0; x<=image.rows; x++)
    {
      if(x==image.rows*6/12)//
      {
        if(image_data[x*image.cols+y]+20<sr&&s==1)//если значение яркости клетки +20 меньше средней яркости изображения
        {
          image_data[x*image.cols+y]=255;//сделать клетку белой
          r1=image.cols-y-(image.cols/2);//расстояние от центра до клетки
        }
        else //иначе
        {
          if(x==image.rows*8/12)//
          {
            if(image_data[x*image.cols+y]+20<sr&&s==2)//если значение яркости клетки +20 меньше средней яркости изображения
            {
              image_data[x*image.cols+y]=255;//сделать клетку белой
              r1=image.cols-y-(image.cols/2);//расстояние от центра до клетки
            }
            else //иначе
            {
              if(x==image.rows*10/12)//
              {
                if(image_data[x*image.cols+y]+20<sr&&s==3)//если значение яркости клетки +20 меньше средней яркости изображения
                {
                  image_data[x*image.cols+y]=255;//сделать клетку белой
                  r1=image.cols-y-(image.cols/2);//расстояние от центра до клетки
                }
                else //иначе
                {
                  fprintf( stderr, "no_R");
                }
              }   
            }
          }
        }
      }
    }
  }
  if (s>=50)
  {
    perehod=1;
  }
  fprintf(stderr, "%i\n", r1);
  raz=r-abs(r1);
  fprintf(stderr, "%i\n", r);
  res1=r1;
  return r;//вернуть изображение
}

