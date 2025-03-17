#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include "pwm/SparkMaxPWM.h"
enum ControlMode {
  LINE_FOLLOW,
  MANUAL,
  STOP
};

class FlightController {
public:
  FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR);
  void update();
  void set_control_mode(ControlMode mode);
  void init();

private:
  SparkMaxPWM &motorL, &motorR;

  ControlMode currentMode;

  void lineFollow();
  void manual();
  void stop();

};

#endif
