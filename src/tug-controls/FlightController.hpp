#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "VL53L.hpp"
#include "pwm/PWMReader.h"
#include "pwm/SparkMaxPWM.h"

enum ControlMode {
  LINE_FOLLOW,
  MANUAL,
  STOP,
  TEST, 
  RC
};

class FlightController {
public:
  FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR, VL53L &tofD, BNO085 &bnoL, AS7341 &clrSensLD, AS7341 &clrSensRD, PWMReader pwmReader);
  void update();
  void set_control_mode(ControlMode mode);
  void init();

private:
  //Passed in 
  SparkMaxPWM &motorL, &motorR;
  VL53L tofD;
  BNO085 bnoL;
  AS7341 clrSensLD, clrSensRD;

  PWMReader pwmReader;

  // Created here
  IMUData IMUDataL;
  ControlMode currentMode;

  void lineFollow();
  void manual();
  void stop();
  void test();
  void rc();

};

#endif
