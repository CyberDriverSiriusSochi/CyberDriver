#include <Motor.h>
//#include <Ultrasonic.h>

class Color
{
  public:
    static const int Red = 9;
    static const int Green = 10;
    static const int Blue = 11;
    int H;
    int S;
    int V;
    int Hi, Vmin, Vinc, Vdec;
    float a, R, G, B;
  public:
    void Init()
    {
      S = 100;
      V = 100;
      H = 0;
      pinMode(Red, OUTPUT);
      pinMode(Green, OUTPUT);
      pinMode(Blue, OUTPUT);
    }
    void color()
    {
      H = (millis() / 50) % 360;
      Hi = int(H / 60) % 6;
      Vmin = (S - 100) * V / 100;
      a = (V - Vmin) * (H % 60) / 60;
      Vinc = Vmin + a;
      Vdec = V - a;
      switch (Hi) {
        case 0:
          R = V;
          G = Vinc;
          B = Vmin;
          break;
        case 1:
          R = Vdec;
          G = V;
          B = Vmin;
          break;
        case 2:
          R = Vmin;
          G = V;
          B = Vinc;
          break;
        case 3:
          R = Vmin;
          G = Vdec;
          B = V;
          break;
        case 4:
          R = Vinc;
          G = Vmin;
          B = V;
          break;
        case 5:
          R = V;
          G = Vmin;
          B = Vdec;
          break;
      }
      R *= 255 / 100.0;
      G *= 255 / 100.0;
      B *= 255 / 100.0;
      analogWrite(Red, round(R));
      analogWrite(Green, round(G));
      analogWrite(Blue, round(B));
    }

};

MotorControl car;
Color col;
/*Ultrasonic ultrasonic1(9, 8);
  Ultrasonic ultrasonic2(11, 10);
  Ultrasonic ultrasonic3(13, 12);*/

void setup() {
  car.Init();
  col.Init();
  Serial.begin(9600);
}

/*void Ultrasensor() {
  Serial.print(ultrasonic1.Ranging((CM)));
  Serial.print(" ");
  Serial.print(ultrasonic2.Ranging((CM)));
  Serial.print(" ");
  Serial.print(ultrasonic3.Ranging((CM)));
  Serial.println(" ");
  }*/


void loop() {

  //Ultrasensor();

  if (Serial.available())
  {

    int left = Serial.parseInt();
    int right = Serial.parseInt();
    /*Serial.print("Left = ");
      Serial.print(left);
      Serial.print("  Right = ");
      Serial.print(right);*/

    if (left > 255)
      left = 255;
    if (left < -255)
      left = -255;

    if (right > 255)
      right = 255;
    if (right < -255)
      right = -255;

    car.SetSpeed(left, right);
    //Ultrasensor();
    while (Serial.available() > 0)
    {
      Serial.read();
    }

  }
  //Serial.flush();
  col.color();
}
