#include <Arduino.h>

class MotorControl
{
  private:
    static const int LeftSpeed = 5;
    static const int LeftDirection = 4;
    static const int RightSpeed = 6;
    static const int RightDirection = 7;
  public:
    MotorControl();
    void SetSpeed(int Left, int Right);
    void Init();
};
