class PID {

  private:
    float ISum;
    float LastInput;
    float P;
    float I;
    float D;
    float value;
  public:
    PID(float _P, float _I, float _D);
    float pid(float target);
};

