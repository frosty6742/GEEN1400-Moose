#include "SparkMaxESC.hpp"
#include <cstring>

SparkMaxESC::SparkMaxESC(uint8_t canID, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16>& canBusRef)
    : sparkMaxID(canID), canBus(canBusRef) {
    sparkMaxCANID = 0x02040000 | sparkMaxID;  // Base CAN ID for Spark MAX
}

void SparkMaxESC::begin() {
    canBus.begin();
    canBus.setBaudRate(1000000);  // Spark MAX default 1 Mbps
}

void SparkMaxESC::setPower(float power) {
    sendCommand(DUTY_CYCLE, power);
}

void SparkMaxESC::setControl(ControlType mode, float value) {
    sendCommand(mode, value);
}

void SparkMaxESC::stop() {
    sendCommand(DUTY_CYCLE, 0.0f);  // Set power to zero to stop motor
}

void SparkMaxESC::sendCommand(ControlType mode, float value) {
    CAN_message_t msg;
    msg.id = sparkMaxCANID;
    msg.len = 8;
    
    msg.buf[0] = static_cast<uint8_t>(mode); // Control mode
    msg.buf[1] = 0x00;  // Unused

    int16_t output = static_cast<int16_t>(value * 32767.0f);
    msg.buf[2] = output & 0xFF;
    msg.buf[3] = (output >> 8) & 0xFF;
    
    msg.buf[4] = 0;  
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0;

    canBus.write(msg);
}

// Read encoder position
float SparkMaxESC::getEncoderPosition() {
    return readResponse(0x08000000 | sparkMaxID);
}

// Read encoder velocity
float SparkMaxESC::getEncoderVelocity() {
    return readResponse(0x09000000 | sparkMaxID);
}

// Read motor current
float SparkMaxESC::getMotorCurrent() {
    return readResponse(0x0C000000 | sparkMaxID);
}

// Read motor temperature
float SparkMaxESC::getMotorTemperature() {
    return readResponse(0x0D000000 | sparkMaxID);
}

// Read faults
uint16_t SparkMaxESC::getFaults() {
    return static_cast<uint16_t>(readResponse(0x0E000000 | sparkMaxID));
}

// Read sticky faults
uint16_t SparkMaxESC::getStickyFaults() {
    return static_cast<uint16_t>(readResponse(0x0F000000 | sparkMaxID));
}

// Reset faults
void SparkMaxESC::resetFaults() {
    CAN_message_t msg;
    msg.id = 0x0E000000 | sparkMaxID;
    msg.len = 8;
    memset(msg.buf, 0, sizeof(msg.buf));
    canBus.write(msg);
}

// Set current limit
void SparkMaxESC::setCurrentLimit(float currentLimit) {
    CAN_message_t msg;
    msg.id = 0x10000000 | sparkMaxID;
    msg.len = 8;
    
    int16_t limit = static_cast<int16_t>(currentLimit * 1000); // Convert to mA
    msg.buf[0] = limit & 0xFF;
    msg.buf[1] = (limit >> 8) & 0xFF;
    msg.buf[2] = 0;
    msg.buf[3] = 0;
    msg.buf[4] = 0;
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0;

    canBus.write(msg);
}

// Read response from Spark MAX
float SparkMaxESC::readResponse(uint32_t paramID) {
    CAN_message_t msg;
    msg.id = paramID;
    msg.len = 8;
    memset(msg.buf, 0, sizeof(msg.buf));

    canBus.write(msg);
    
    CAN_message_t response;
    uint32_t timeout = millis() + 100;
    while (millis() < timeout) {
        if (canBus.read(response)) {
            int16_t rawValue = (response.buf[2] | (response.buf[3] << 8));
            return rawValue / 32767.0f;  // Convert back to float
        }
    }
    return -1; // Return an invalid value if no response received
}

