#pragma once

#include "SparkMaxESC.hpp"
#include <FlexCAN_T4.h>

class MotorController {
public:
  MotorController(uint8_t leftMotorID, uint8_t rightMotorID,
                  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus);

  void begin();
  void setMotorSpeed(float left, float right);
  void setThrottle(float throttle);
  void setSteering(float steering);
  void setThrottleAndSteering(float throttle, float steering);
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
