#include "SparkMaxESC.hpp"

SparkMaxESC::SparkMaxESC(uint8_t canID) : sparkMaxID(canID) {
  sparkMaxCANID =
      (0x02040000 |
       (sparkMaxID & 0x3F)); // Ensure correct Spark MAX ID (6-bit device ID)
}

void SparkMaxESC::begin() {
  Serial.println("Initializing SparkMaxESC...");
  CANManager::initialize(); // Ensure CAN bus is initialized only once
}

void SparkMaxESC::setPower(float power) {
  Serial.print("Setting power: ");
  Serial.println(power);
  sendCommand(DUTY_CYCLE, power);
}

void SparkMaxESC::setControl(ControlType mode, float value) {
  Serial.print("Setting control mode: ");
  Serial.print(static_cast<uint8_t>(mode));
  Serial.print(", Value: ");
  Serial.println(value);
  sendCommand(mode, value);
}

void SparkMaxESC::stop() {
  Serial.println("Stopping motor.");
  sendCommand(DUTY_CYCLE, 0.0f);
}

void SparkMaxESC::sendCommand(ControlType mode, float value) {
  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = sparkMaxCANID;
  msg.len = 8;
  msg.flags.extended = 1; // 29-bit extended CAN ID

  msg.buf[0] = static_cast<uint8_t>(mode);
  msg.buf[1] = 0x00; // Unused, must be zero

  int16_t output = static_cast<int16_t>(value * 32767.0f);
  msg.buf[2] = output & 0xFF;
  msg.buf[3] = (output >> 8) & 0xFF;

  for (int i = 4; i < 8; i++) {
    msg.buf[i] = 0;
  }

  Serial.print("Sending CAN message with ID: ");
  Serial.println(msg.id, HEX);

  if (canBus.write(msg)) {
    Serial.println("CAN message sent successfully.");
  } else {
    Serial.println("CAN message send failed!");
  }
}

// Read encoder position
float SparkMaxESC::getEncoderPosition() {
  return readResponse(0x08000000 | (sparkMaxID & 0x3F));
}

// Read encoder velocity
float SparkMaxESC::getEncoderVelocity() {
  return readResponse(0x09000000 | (sparkMaxID & 0x3F));
}

// Read motor current
float SparkMaxESC::getMotorCurrent() {
  return readResponse(0x0C000000 | (sparkMaxID & 0x3F));
}

// Read motor temperature
float SparkMaxESC::getMotorTemperature() {
  return readResponse(0x0D000000 | (sparkMaxID & 0x3F));
}

// Read faults
uint16_t SparkMaxESC::getFaults() {
  return static_cast<uint16_t>(readResponse(0x0E000000 | (sparkMaxID & 0x3F)));
}

// Read sticky faults
uint16_t SparkMaxESC::getStickyFaults() {
  return static_cast<uint16_t>(readResponse(0x0F000000 | (sparkMaxID & 0x3F)));
}

// Reset faults
void SparkMaxESC::resetFaults() {
  Serial.println("Resetting faults...");
  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = 0x0E000000 | (sparkMaxID & 0x3F);
  msg.len = 8;
  msg.flags.extended = 1; // Extended ID

  for (int i = 0; i < 8; i++) {
    msg.buf[i] = 0;
  }

  if (canBus.write(msg)) {
    Serial.println("Fault reset command sent successfully.");
  } else {
    Serial.println("Fault reset command failed!");
  }
}

// Set current limit
void SparkMaxESC::setCurrentLimit(float currentLimit) {
  Serial.print("Setting current limit: ");
  Serial.println(currentLimit);

  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = 0x10000000 | (sparkMaxID & 0x3F);
  msg.len = 8;
  msg.flags.extended = 1; // Extended ID

  int16_t limit = static_cast<int16_t>(currentLimit * 1000); // Convert to mA
  msg.buf[0] = limit & 0xFF;
  msg.buf[1] = (limit >> 8) & 0xFF;

  for (int i = 2; i < 8; i++) {
    msg.buf[i] = 0;
  }

  if (canBus.write(msg)) {
    Serial.println("Current limit set successfully.");
  } else {
    Serial.println("Failed to set current limit!");
  }
}

// Read response from Spark MAX
float SparkMaxESC::readResponse(uint32_t paramID) {
  Serial.print("Requesting data with ID: ");
  Serial.println(paramID, HEX);

  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = paramID;
  msg.len = 8;
  msg.flags.extended = 1; // Extended ID

  for (int i = 0; i < 8; i++) {
    msg.buf[i] = 0;
  }

  if (!canBus.write(msg)) {
    Serial.println("Failed to send read request!");
    return -1;
  }

  CAN_message_t response;
  uint32_t start_time = millis();
  while (millis() - start_time < 50) {
    if (canBus.read(response)) {
      int16_t rawValue = (response.buf[2] | (response.buf[3] << 8));
      return rawValue / 32767.0f;
    }
    delay(1);
  }

  Serial.println("No response received.");
  return -1;
}
