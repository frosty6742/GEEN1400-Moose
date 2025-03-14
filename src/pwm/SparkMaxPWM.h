#ifndef SPARK_MAX_PWM_H
#define SPARK_MAX_PWM_H

#include <Arduino.h>

class SparkMaxPWM {
public:
    SparkMaxPWM(int pwmPin);   // Constructor
    void begin();              // Initialize PWM
    void setSpeed(float speed); // Set motor speed (-1.0 to 1.0)

private:
    int _pwmPin;
    int _minPulse = 1000; // Microseconds for full reverse
    int _maxPulse = 2000; // Microseconds for full forward
};

#endif

