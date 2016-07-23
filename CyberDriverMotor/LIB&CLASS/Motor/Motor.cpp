#include <Arduino.h>
#include <Motor.h>


MotorControl::MotorControl()
{

}
void MotorControl::SetSpeed(int Left, int Right)
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
    digitalWrite(RightDirection, LOW);
    analogWrite(RightSpeed, Right);
  }
  else
  {
    digitalWrite(RightDirection, HIGH);
    analogWrite(RightSpeed, Right * (-1));
  }
}

void MotorControl::Init()
{
  pinMode(LeftSpeed, OUTPUT);
  pinMode(LeftDirection, OUTPUT);
  pinMode(RightSpeed, OUTPUT);
  pinMode(RightDirection, OUTPUT);
}

