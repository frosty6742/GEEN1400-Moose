#include "MotorController.hpp"
#include "usb_serial.h"
#include <cmath>

MotorController::MotorController(uint8_t leftMotorID, uint8_t rightMotorID)
    : leftMotor(leftMotorID), rightMotor(rightMotorID) {}

void MotorController::begin() {
  Serial.println("Initializing MotorController...");
  CANManager::initialize(); // Ensure CAN Bus is initialized only once
  leftMotor.begin();
  rightMotor.begin();
}

void MotorController::setMotorSpeed(float left, float right) {
  if (left > 1 || left < -1) {
    left = 0;
    Serial.println("Invalid left motor speed set in MotorController.cpp");
  }

  if (right > 1 || right < -1) {
    right = 0;
    Serial.println("Invalid right motor speed set in MotorController.cpp");
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

  Serial.print("Velocity: ");
  Serial.println(getLeftVelocity());

  Serial.print("Current: ");
  Serial.println(getLeftCurrent());

  Serial.print("Temperature: ");
  Serial.println(getLeftTemperature());

  Serial.print("Faults: ");
  Serial.println(getLeftFaults());

  Serial.println("Right Motor:");
  Serial.print("Encoder Pos: ");
  Serial.println(getRightEncoderPosition());

  Serial.print("Velocity: ");
  Serial.println(getRightVelocity());

  Serial.print("Current: ");
  Serial.println(getRightCurrent());

  Serial.print("Temperature: ");
  Serial.println(getRightTemperature());

  Serial.print("Faults: ");
  Serial.println(getRightFaults());
}
