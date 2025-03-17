#include "FlightController.hpp"
#include "core_pins.h"
#include "pwm/SparkMaxPWM.h"




FlightController::FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR)
    : motorL(motorL), motorR(motorR), currentMode(STOP)  {}

void FlightController::set_control_mode(ControlMode mode) {
  currentMode = mode;
}

void FlightController::update() {
  switch (currentMode) {
  case LINE_FOLLOW:
    lineFollow();
    break;
  case  MANUAL:
    manual();
    break;
  case STOP:
    stop();
    break;
  }
}

void FlightController::init() {
  motorL.begin();
  motorR.begin();
  delay(1000);
}

// Mode 1: lineFollow
void FlightController::lineFollow() {

}

//Mode 2: manual drive
void FlightController::manual() {

}
