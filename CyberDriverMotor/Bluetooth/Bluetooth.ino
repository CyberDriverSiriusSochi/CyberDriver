class MotorControl
{
  private:
    static const int LeftSpeed = 5;
    static const int LeftDirection = 4;
    static const int RightSpeed = 6;
    static const int RightDirection = 7;
  public:
    MotorControl()
    {

    }
    void SetSpeed(int Left, int Right)
    {
      if (Left != 0)
      {
        if (Left > 0)
          Left = map(Left, 0, 255, 90, 255);
        else
          Left = map(Left, 0, -255, -90, -255);
      }
      if (Right != 0)
      {
        if (Right > 0)
          Right = map(Right, 0, 255, 90, 255);
        else
          Right = map(Right, 0, -255, -90, -255);
      }
      if (Left > 0)
      {
        digitalWrite(LeftDirection, HIGH);
        analogWrite(LeftSpeed, Left);
      }
      else
      {
        digitalWrite(LeftDirection, LOW);
        analogWrite(LeftSpeed, Left * (-1));
      }
      if (Right > 0)
      {
        digitalWrite(RightDirection, HIGH);
        analogWrite(RightSpeed, Right);
      }
      else
      {
        digitalWrite(RightDirection, LOW);
        analogWrite(RightSpeed, Right * (-1));
      }
    }

    void Init()
    {
      pinMode(LeftSpeed, OUTPUT);
      pinMode(LeftDirection, OUTPUT);
      pinMode(RightSpeed, OUTPUT);
      pinMode(RightDirection, OUTPUT);
    }

};

class PID {

  private:
    float ISum;
    float LastInput;
    float P;
    float I;
    float D;
    float value;
  public:
    PID()
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

MotorControl MC;
PID pidL;
PID pidR;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello, world!");
  MC.Init();
  //for (int i = 64; i < 128; i++)
  //{
  //  MC.SetSpeed(i, i);
  //  Serial.println(i);
  //  delay(300);
  //}
  delay(3000);
}

float taskListL[5] = {255.0, 255.0, -255.0, 100.0, -255.0};
float taskListR[5] = {255.0, -255.0, 255.0, 100.0, -255.0};

void loop(void)
{
  Serial.println("Hello, world!");
  delay(300);
  for (int j = 0; j < (sizeof(taskListL) / sizeof(taskListL[0])); j++)
  {
    for (int i = 0; i < 100; ++i)
    {
      MC.SetSpeed(pidL.pid(taskListL[j]), pidR.pid(taskListR[j]));
      delay(20);
    }
  }
  return;
  MC.SetSpeed(255, 255);
  delay(5000);
  MC.SetSpeed(255, -255);
  delay(5000);
  MC.SetSpeed(-255, -255);
  delay(5000);
  MC.SetSpeed(0, 0);
  delay(5000);
}

