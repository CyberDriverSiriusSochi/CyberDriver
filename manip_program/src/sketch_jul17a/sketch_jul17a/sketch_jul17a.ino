#include <Servo.h>
#include <Wire.h>
#include <Multiservo.h>

#define countServo 4
Multiservo servo[countServo];
int  pins[countServo] = {0, 1, 2, 3};

class Manipulator
{
  private:

  public:
    Manipulator()
    {

    }
    static void Init()
    {
      for (int i = 0; i < countServo; i++)
        servo[i].attach(pins[i]);
    }
    static void SetDegree(int number, int degree)
    {
      servo[number].write(degree);
    }
    static void SetPosition(int first, int second, int third, int fourth)
    {
      servo[0].write(first);
      servo[1].write(second);
      servo[2].write(third);
      servo[3].write(fourth);
    }
};

class Manpul: public Manipulator
{

};

void setup(void)
{
  Wire.begin();
  Manpul::Init();
  Serial.begin(9600);
  servo[0].write(90);
  servo[1].write(90);
  servo[2].write(0);
  servo[3].write(90);
  delay(1000);
  servo_3.write(45);
  delay(200);
  servo_3.write(135);
  delay(200);
  servo_3.write(45);
  delay(200);
  servo_3.write(90);
}
}

void loop(void)
{
  if (Serial.available())
  {
    Manpul::SetPosition(Serial.parseInt(), Serial.parseInt() , Serial.parseInt()*(-1), Serial.parseInt()+90);

    while (Serial.available())
      Serial.read();

  }
}


