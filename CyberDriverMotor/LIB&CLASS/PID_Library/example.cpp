#include <cstdio>
#include <math.h>

class PID {

  private:
    float ISum;
    float LastInput;
    float P;
    float I;
    float D;
    float value;
  public:
    PID(float _P, float _I, float _D)
    {
      P = 0.1;
      I = 0.0;
      D = -0.3;

      ISum = 0.0;
      LastInput = 0.0;

      value = 0.0;
    }

    float pid(float target)
    {
      ISum += (target - value);
      float result = P * (target - value) + I * ISum + D * (value - LastInput);
      LastInput = value;
      value += result;
      return value;
    }
};

float taskList[5] = {155.0, 255.0, -255.0, 255.0, 0.0};

int main()
{
  FILE *fout = fopen("file.csv", "wt");
  PID f(0.1, 0.0, -0.3);
  
  float value = 0;
  for(int j = 0; j < (sizeof(taskList) / sizeof(taskList[0])); j++)
  {
    for(int i = 0; i < 100; ++i)
    {
      value = f.pid(taskList[j]);
      printf("%f\n", value);
      fprintf(fout,"%f\n",value);
    }
  }
  fclose(fout);
  return 0;
}
