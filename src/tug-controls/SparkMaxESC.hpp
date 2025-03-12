#pragma once

#include "CANManager.hpp"

enum ControlType {
    DUTY_CYCLE = 0,
    VELOCITY = 1,
    POSITION = 2,
    CURRENT = 3
};

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

private:
    uint8_t sparkMaxID;
    uint32_t sparkMaxCANID;

    void sendCommand(ControlType mode, float value);
    float readResponse(uint32_t paramID);
};


