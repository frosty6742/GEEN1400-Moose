#pragma once

#include "SparkMaxESC.hpp"

class MotorController {
public:
  MotorController(uint8_t leftMotorID, uint8_t rightMotorID);

  void begin();
  void setMotorSpeed(float left, float right);
  void setMotorCurrentLimit(float left, float right);
  void emergencyStop();

  float getLeftEncoderPosition();
  float getRightEncoderPosition();
  float getLeftVelocity();
  float getRightVelocity();
  float getLeftCurrent();
  float getRightCurrent();
  float getLeftTemperature();
  float getRightTemperature();
  uint16_t getLeftFaults();
  uint16_t getRightFaults();

  void printAllReadings();

private:
  SparkMaxESC leftMotor;
  SparkMaxESC rightMotor;
};
