#include <cstdio>
#include <math.h>

class SinReg{
  private:
    int Tick;
    int value;
    int target;
    float scaleX;
    float scaleY;
    float result;
    int curTick;
    static const int maxTick = 100;
    static const float pi = 3.14159265;
    static const int TickCount = 5;

  public:
    SinReg()
    {
      Tick = 0.8 * maxTick;
      curTick = Tick;
      value = 0;
      result = 0;
      target = 0;
    }
    
    void NewTarget(float targ)
    {
      Tick = (Tick * (TickCount - 1) + curTick) / TickCount;
      curTick = 0;
      value = result;
      target = targ;
      scaleX = Tick / pi;
      scaleY = (target - value) / 2.0;
    }

    float next()
    {
      if (curTick >= Tick)
      {
        result = target;
      }
      else
      {
        result = value + scaleY * (sin((pi * curTick / Tick) - pi / 2.0) + 1);
      }
      curTick++;
      return result;
    }
};

float taskList[5] = {20.0, 10.0, 20.0, 30.0, 5.0};

int main()
{
  FILE *fout = fopen("file.csv", "wt");
  
  SinReg sinr;
  float value = 0;
  for(int j = 0; j < (sizeof(taskList) / sizeof(taskList[0])); j++)
  {
    sinr.NewTarget(taskList[j]);
    for(int i = 0; i < 100; ++i)
    {
      value = sinr.next();
      printf("%f\n", value);
      fprintf(fout,"%f\n",value);
    }
  }
  fclose(fout);
  return 0;
}
