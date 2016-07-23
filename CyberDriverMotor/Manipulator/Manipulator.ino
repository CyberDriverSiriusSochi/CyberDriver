#include <Wire.h>
#include <Multiservo.h>

#define countServo 4
Multiservo servo[countServo];
int  pins[countServo] = {0, 13, 14, 15};

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
  Manpul::SetPosition(0,0,0,90);
}

void loop(void)
{
  if (Serial.available())
  {
    Manpul::SetPosition(Serial.parseInt(), Serial.parseInt(), Serial.parseInt(), Serial.parseInt());

    while (Serial.available())
      Serial.read();

  }
  //for(int i = 
  Manpul::SetPosition(0,20,80,50);
  delay(500);
  Manpul::SetPosition(0,20,70,60);
  delay(500);
  /*Manpul::SetPosition(0,20,80,70);
  delay(500);
  Manpul::SetPosition(0,20,80,70);
  delay(500);*/
  
}
