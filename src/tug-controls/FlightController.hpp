#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include "VL53L.hpp"
#include "pwm/SparkMaxPWM.h"
enum ControlMode {
  LINE_FOLLOW,
  MANUAL,
  STOP
};

class FlightController {
public:
  FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR, VL53L &tofD);
  void update();
  void set_control_mode(ControlMode mode);
  void init();

private:
  SparkMaxPWM &motorL, &motorR;
  VL53L tofD;

  ControlMode currentMode;

  void lineFollow();
  void manual();
  void stop();

};

#endif
