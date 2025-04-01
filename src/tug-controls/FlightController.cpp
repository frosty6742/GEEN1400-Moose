#include "FlightController.hpp"
#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "VL53L.hpp"
#include "core_pins.h"
#include "pwm/SparkMaxPWM.h"
#include "usb_serial.h"

FlightController::FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR,
                                   VL53L &tofD, BNO085 &bnoL, AS7341 &clrSensLD,
                                   AS7341 &clrSensRD)
    : motorL(motorL), motorR(motorR), tofD(tofD), bnoL(bnoL),
      clrSensLD(clrSensLD), clrSensRD(clrSensRD), currentMode(LINE_FOLLOW) {}

void FlightController::set_control_mode(ControlMode mode) {
  Serial.print("Control Mode set to: ");
  Serial.println(mode);
  currentMode = mode;
}

void FlightController::update() {
  switch (currentMode) {
  case LINE_FOLLOW:
    lineFollow();
    break;
  case MANUAL:
    manual();
    break;
  case STOP:
    stop();
    break;
  case TEST:
    test();
    break;
  }
}

void FlightController::init() {
  motorL.begin();
  motorR.begin();
  tofD.init();
  bnoL.begin();
  clrSensRD.begin();
  clrSensLD.begin();

  Serial.println("Flight Controller Started.");

  delay(1000);
}

// Mode 1: lineFollow
void FlightController::lineFollow() {

  float dist = tofD.getDistance();

  if (dist < 20 && dist > 5) { // Turn
    motorL.setSpeed(-0.1);
    motorL.setSpeed(0.1);
    delay(2000);
  } else if (dist < 5) {
    motorL.setSpeed(0.0);
    motorR.setSpeed(0.0);
  } else {
    motorL.setSpeed(0.2);
    motorR.setSpeed(0.2);
  }
}

// Mode 2: manual drive
void FlightController::manual() {}

void FlightController::stop() {}

void FlightController::test() {
  Serial.println("Testing Sensors");

  float dist = tofD.getDistance();
  Serial.println(dist);

  // bnoDataL = bnoL.getIMUData();
  bnoL.printIMUData();

  clrSensRD.printData();
}
