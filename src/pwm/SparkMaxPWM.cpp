#include "SparkMaxPWM.h"
#include "usb_serial.h"

SparkMaxPWM::SparkMaxPWM(int pwmPin) { _pwmPin = pwmPin; }

void SparkMaxPWM::begin() {
  pinMode(_pwmPin, OUTPUT);
  analogWriteFrequency(_pwmPin, 50); // 50Hz PWM for motor control
  Serial.println("SparkMaxPWM initilized.");
}

void SparkMaxPWM::setSpeed(float speed) {
  speed = constrain(speed, -1.0, 1.0); // Limit speed to -1 to 1
  int pulseWidth =
      map(speed * 100, -100, 100, 1000, 2000); // Map to 1000-2000µs

  // Convert microseconds to a value suitable for analogWrite()
  int dutyCycle = (pulseWidth * 255) / 20000;
  analogWrite(_pwmPin, dutyCycle);
}
