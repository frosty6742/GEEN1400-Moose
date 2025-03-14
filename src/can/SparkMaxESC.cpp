
#include "SparkMaxESC.hpp"

SparkMaxESC::SparkMaxESC(uint8_t canID) : sparkMaxID(canID) {
  sparkMaxCANID =
      0x02040000 | (sparkMaxID & 0x3F); // Correctly formatted Extended CAN ID
}

void SparkMaxESC::begin() {
  Serial.println("Initializing SparkMaxESC...");
  CANManager::initialize(); // Ensure CAN bus is initialized only once
}

void SparkMaxESC::setPower(float power) {
  Serial.print("Setting power: ");
  Serial.println(power);
  sendCommand(0x02040000, sparkMaxID, 0x00, power); // 0x00 = Duty Cycle Mode
}

void SparkMaxESC::setControl(ControlType mode, float value) {
  Serial.print("Setting control mode: ");
  Serial.print(static_cast<uint8_t>(mode));
  Serial.print(", Value: ");
  Serial.println(value);
  sendCommand(0x02040000, sparkMaxID, static_cast<uint8_t>(mode), value);
}

void SparkMaxESC::stop() {
  Serial.println("Stopping motor.");
  sendCommand(0x02040000, sparkMaxID, 0x00, 0.0f);
}

void SparkMaxESC::sendCommand(uint32_t baseID, uint8_t deviceID,
                              uint8_t controlMode, float value) {
  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = baseID | (deviceID & 0x3F); // Correctly formatted Extended CAN ID
  msg.len = 8;
  msg.flags.extended = 1; // 29-bit Extended CAN ID

  // Encode data
  msg.buf[0] = controlMode; // Control Mode (e.g., 0x00 = Duty Cycle)
  msg.buf[1] = 0x00;        // Unused, must be 0

  int16_t output = static_cast<int16_t>(
      value * 32767.0f); // Convert float (-1 to 1) to Q15 format
  msg.buf[2] = output & 0xFF;
  msg.buf[3] = (output >> 8) & 0xFF;

  // Zero out unused bytes
  for (int i = 4; i < 8; i++) {
    msg.buf[i] = 0;
  }

  // Debugging output
  Serial.print("Sending CAN message with ID: ");
  Serial.println(msg.id, HEX);

  Serial.print("Data: ");
  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Send message
  if (canBus.write(msg)) {
    Serial.println("CAN message sent successfully.");
  } else {
    Serial.println("CAN message send failed!");
  }
}

// Read encoder position
float SparkMaxESC::getEncoderPosition() {
  return readResponse(0x08000000, sparkMaxID);
}

// Read encoder velocity
float SparkMaxESC::getEncoderVelocity() {
  return readResponse(0x09000000, sparkMaxID);
}

// Read motor current
float SparkMaxESC::getMotorCurrent() {
  return readResponse(0x0C000000, sparkMaxID);
}

// Read motor temperature
float SparkMaxESC::getMotorTemperature() {
  return readResponse(0x0D000000, sparkMaxID);
}

// Read faults
uint16_t SparkMaxESC::getFaults() {
  return static_cast<uint16_t>(readResponse(0x0E000000, sparkMaxID));
}

// Read sticky faults
uint16_t SparkMaxESC::getStickyFaults() {
  return static_cast<uint16_t>(readResponse(0x0F000000, sparkMaxID));
}

// Reset faults
void SparkMaxESC::resetFaults() {
  Serial.println("Resetting faults...");
  sendCommand(0x0E000000, sparkMaxID, 0x00, 0.0f);
}

// Set current limit
void SparkMaxESC::setCurrentLimit(float currentLimit) {
  Serial.print("Setting current limit: ");
  Serial.println(currentLimit);

  int16_t limit = static_cast<int16_t>(currentLimit * 1000); // Convert to mA
  sendCommand(0x10000000, sparkMaxID, 0x00, limit);
}

// Read response from SPARK MAX
float SparkMaxESC::readResponse(uint32_t paramID, uint8_t deviceID) {
  Serial.print("Requesting data with ID: ");
  Serial.println(paramID, HEX);

  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();

  CAN_message_t msg;
  msg.id = paramID | (deviceID & 0x3F); // Ensure correct 29-bit ID
  msg.len = 8;
  msg.flags.extended = 1; // Extended ID

  for (int i = 0; i < 8; i++) {
    msg.buf[i] = 0;
  }

  Serial.print("Sending Read Request ID: ");
  Serial.println(msg.id, HEX);

  if (!canBus.write(msg)) {
    Serial.println("Failed to send read request!");
    return -1;
  }

  // Wait for response
  CAN_message_t response;
  uint32_t start_time = millis();
  while (millis() - start_time < 50) {
    if (canBus.read(response)) {
      Serial.print("Received Response ID: ");
      Serial.println(response.id, HEX);

      Serial.print("Response Data: ");
      for (int i = 0; i < response.len; i++) {
        Serial.print(response.buf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Assume it's a 32-bit IEEE-754 float (for position, velocity, etc.)
      float result;
      memcpy(&result, &response.buf[0], sizeof(result));
      return result;
    }
    delay(1);
  }

  Serial.println("No response received.");
  return -1;
}

// Listen for any CAN messages (for debugging)
void SparkMaxESC::listenForCanMessages() {
  FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();
  CAN_message_t msg;

  Serial.println("Listening for incoming CAN messages...");
  while (true) {
    if (canBus.read(msg)) {
      Serial.print("Received CAN ID: ");
      Serial.println(msg.id, HEX);

      Serial.print("Data: ");
      for (int i = 0; i < msg.len; i++) {
        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    delay(10);
  }
}
