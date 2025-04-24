#include "wiring.h"
#include "../libraries/PWMServo-2.1/PWMServo.h"

class SparkMaxPWM {
  public:
    SparkMaxPWM(int pwmPin);
    void begin();
    void setSpeed(float speed);

  private:
    int _pwmPin;
    PWMServo _servo;
};

SparkMaxPWM::SparkMaxPWM(int pwmPin) {
  _pwmPin = pwmPin;
}

void SparkMaxPWM::begin() {
  _servo.attach(_pwmPin, 1000, 2000); // Attach with custom min/max
}

void SparkMaxPWM::setSpeed(float speed) {
  speed = constrain(speed, -1.0, 1.0);
  int angle = map(speed * 100, -100, 100, 0, 180);
  _servo.write(angle);
}


