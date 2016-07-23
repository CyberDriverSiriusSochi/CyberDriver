#include <cstdio>
#include <math.h>

class PID{

  private:
    float ISum;
    float LastInput;
    float LastOutput;
    static const float P = 0.1;//0.2
    static const float I = 0;//0.0001;//0.0002;
    static const float D = -0.3;//-0.5;
  public:
  PID()
  {
    ISum = 0.0;
    LastInput = 0.0;
    LastOutput = 0.0;
  }

  float pid(float value, float target)
  {
    ISum += (target - value);
    float result = P * (target - value) + I * ISum + D * (value - LastInput);
    LastInput = value;
    LastOutput = result;
    return result;
  }
};

float taskList[5] = {20.0, 10.0, 20.0, 30.0, 5.0};

int main()
{
  FILE *fout = fopen("file.csv", "wt");
  PID f;
  
  float value = 0;
  for(int j = 0; j < (sizeof(taskList) / sizeof(taskList[0])); j++)
  {
    for(int i = 0; i < 100; ++i)
    {
      value += f.pid(value, taskList[j]);
      printf("%f\n", value);
      fprintf(fout,"%f\n",value);
    }
  }
  fclose(fout);
  return 0;
}
