#pragma once

#include <FlexCAN_T4.h>

enum ControlType {
  DUTY_CYCLE = 0,
  VELOCITY = 1,
  POSITION = 2,
  CURRENT = 3,
  VOLTAGE = 4
};

class SparkMaxESC {
public:
  SparkMaxESC(uint8_t canID, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus);

  void begin();
  void setPower(float power);
  void setControl(ControlType mode, float value);
  void stop();

  // Sensor Read Functions
  float getEncoderPosition();
  float getEncoderVelocity();
  float getMotorCurrent();
  float getMotorTemperature();
  uint16_t getFaults();
  uint16_t getStickyFaults();

  // Configuration & Fault Management
  void resetFaults();
  void setCurrentLimit(float currentLimit);

private:
  uint8_t sparkMaxID;
  uint32_t sparkMaxCANID;
  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus;

  void sendCommand(ControlType mode, float value);
  float readResponse(uint32_t paramID);
};

