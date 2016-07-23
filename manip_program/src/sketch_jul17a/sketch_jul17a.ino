#include <Servo.h>
Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;
void setup()
{
  servo_0.attach(0);
  servo_1.attach(1);
  servo_2.attach(2);
  servo_3.attach(3);
  servo_0.write(90);
  servo_1.write(90);
  servo_2.write(0);
  servo_3.write(90);
  delay(1000);
  servo_3.write(45);
  delay(200);
  servo_3.write(135);
  delay(200);
  servo_3.write(45);
  delay(200);
  servo_3.write(90);
}
void loop()
{
 servo_0.write(90);
  servo_1.write(90);
  servo_2.write(0);
  servo_3.write(90);   
  
}
