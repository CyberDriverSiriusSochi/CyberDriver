#include <PID.h>
    PID::PID(float _P, float _I, float _D)
    {
      P = _P;
      I = _I;
      D = _D;

      ISum = 0.0;
      LastInput = 0.0;

      value = 0.0;
    }

    float PID::pid(float target)
    {
      ISum += (target - value);
      float result = P * (target - value) + I * ISum + D * (value - LastInput);
      LastInput = value;
      value += result;
      return value;
    }
