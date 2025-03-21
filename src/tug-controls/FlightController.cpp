#include "FlightController.hpp"
#include "VL53L.hpp"
#include "core_pins.h"
#include "pwm/SparkMaxPWM.h"

FlightController::FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR,
                                   VL53L &tofD)
    : motorL(motorL), motorR(motorR), tofD(tofD), currentMode(LINE_FOLLOW) {}

void FlightController::set_control_mode(ControlMode mode) {
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
    // stop();
    break;
  }
}

void FlightController::init() {
  motorL.begin();
  motorR.begin();
  tofD.init();

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
