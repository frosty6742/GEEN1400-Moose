#include "MotorController.hpp"
#include "usb_serial.h"
#include <cmath>

MotorController::MotorController(
    uint8_t leftMotorID, uint8_t rightMotorID,
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus)
    : leftMotor(leftMotorID, canBus), rightMotor(rightMotorID, canBus) {}

void MotorController::begin() {
  leftMotor.begin();
  rightMotor.begin();
}

void MotorController::setMotorSpeed(float left, float right) {
  if (left > 1 || left < 1) {
    left = 0;
    Serial.print("Invalid left motor speed set in "
                 "./src/tug-controlls/MotorController.cpp");
  }

  if (right > 1 || right < 1) {
    right = 0;
    Serial.print("Invalid right motor speed set in "
                 "./src/tug-controlls/MotorController.cpp");
  }

  leftMotor.setPower(left);
  rightMotor.setPower(right);
}

void MotorController::setMotorCurrentLimit(float left, float right) {
  leftMotor.setCurrentLimit(left);
  rightMotor.setCurrentLimit(right);
}

void MotorController::emergencyStop() {
  leftMotor.stop();
  rightMotor.stop();
}

// Sensor Readings
float MotorController::getLeftEncoderPosition() {
  return leftMotor.getEncoderPosition();
}
float MotorController::getRightEncoderPosition() {
  return rightMotor.getEncoderPosition();
}
float MotorController::getLeftVelocity() {
  return leftMotor.getEncoderVelocity();
}
float MotorController::getRightVelocity() {
  return rightMotor.getEncoderVelocity();
}
float MotorController::getLeftCurrent() { return leftMotor.getMotorCurrent(); }
float MotorController::getRightCurrent() {
  return rightMotor.getMotorCurrent();
}
float MotorController::getLeftTemperature() {
  return leftMotor.getMotorTemperature();
}
float MotorController::getRightTemperature() {
  return rightMotor.getMotorTemperature();
}
uint16_t MotorController::getLeftFaults() { return leftMotor.getFaults(); }
uint16_t MotorController::getRightFaults() { return rightMotor.getFaults(); }

void MotorController::printAllReadings() {
  Serial.println("Left Motor:");
  Serial.print("Encoder Pos: ");
  Serial.println(getLeftEncoderPosition());

  Serial.print("Veloicty:");
  Serial.println(getLeftVelocity());

  Serial.print("Current: ");
  Serial.println(getLeftCurrent());

  Serial.print("Tempature: ");
  Serial.println(getLeftTemperature());

  Serial.print("Faults: ");
  Serial.println(getLeftFaults());
}
