
#pragma once

#include "CANManager.hpp"

enum ControlType { DUTY_CYCLE = 0, VELOCITY = 1, POSITION = 2, CURRENT = 3 };

class SparkMaxESC {
public:
  SparkMaxESC(uint8_t canID);

  void begin();
  void setPower(float power);
  void setControl(ControlType mode, float value);
  void stop();

  float getEncoderPosition();
  float getEncoderVelocity();
  float getMotorCurrent();
  float getMotorTemperature();

  uint16_t getFaults();
  uint16_t getStickyFaults();
  void resetFaults();

  void setCurrentLimit(float currentLimit);
  void listenForCanMessages();

private:
  uint8_t sparkMaxID;
  uint32_t sparkMaxCANID;

  void sendCommand(uint32_t baseID, uint8_t deviceID, uint8_t controlMode, float value);
  float readResponse(uint32_t paramID, uint8_t deviceID);
};

