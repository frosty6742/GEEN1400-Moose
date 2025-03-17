#include "SparkMaxESC.hpp"
#include <Arduino.h>

SparkMaxESC::SparkMaxESC(uint8_t canID) : sparkMaxID(canID) {
  sparkMaxCANID =
      0x02040000 | (sparkMaxID & 0x3F); // Correct Extended CAN ID format
}

void SparkMaxESC::begin() {
  Serial.println("Initializing SparkMaxESC...");
  CANManager::initialize();
  delay(100); // Allow time for bus stabilization

  // Test CAN Bus by sending a test message
  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();
  CAN_message_t msg;
  msg.id = 0x123; // Arbitrary test ID
  msg.len = 8;
  msg.flags.extended = 0;
  for (int i = 0; i < 8; i++)
    msg.buf[i] = i;

  Serial.print("Testing CAN Bus: Sending message...");
  if (canBus.write(msg)) {
    Serial.println("Success!");
  } else {
    Serial.println("FAILED: Check wiring and initialization.");
  }
}

void SparkMaxESC::setPower(float power) {
  Serial.print("Setting power: ");
  Serial.println(power);
  sendCommand(0x01000000, sparkMaxID, 0x00, power); // Correct API ID
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

void SparkMaxESC::sendCommand(uint32_t apiID, uint8_t deviceID,
                              uint8_t controlMode, float value) {
  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();
  CAN_message_t msg;
  msg.id = (deviceID & 0x3F) | (apiID << 6); // SPARK MAX follows this format
  msg.len = 8;
  msg.flags.extended = 1;

  msg.buf[0] = controlMode;
  msg.buf[1] = 0x00;
  int16_t output = static_cast<int16_t>(value * 32767.0f);
  msg.buf[2] = output & 0xFF;
  msg.buf[3] = (output >> 8) & 0xFF;

  for (int i = 4; i < 8; i++) {
    msg.buf[i] = 0;
  }

  Serial.print("Sending CAN message with ID: ");
  Serial.println(msg.id, HEX);
  Serial.print("Data: ");
  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (canBus.write(msg)) {
    Serial.println("CAN message sent successfully.");
  } else {
    Serial.println("CAN message send failed!");
  }
}

float SparkMaxESC::readResponse(uint32_t paramID, uint8_t deviceID) {
  Serial.print("Requesting data with ID: ");
  Serial.println(paramID, HEX);
  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();
  CAN_message_t msg;
  msg.id = paramID | (deviceID & 0x3F);
  msg.len = 8;
  msg.flags.extended = 1;

  for (int i = 0; i < 8; i++) {
    msg.buf[i] = 0;
  }

  Serial.print("Sending Read Request ID: ");
  Serial.println(msg.id, HEX);

  if (!canBus.write(msg)) {
    Serial.println("Failed to send read request!");
    return -1;
  }

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
      float result;
      memcpy(&result, &response.buf[0], sizeof(result));
      return result;
    }
    delay(1);
  }

  Serial.println("No response received.");
  return -1;
}

void SparkMaxESC::listenForCanMessages() {
  FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &canBus = CANManager::getInstance();
  CAN_message_t msg;

  Serial.println("Listening for ALL incoming CAN messages...");
  while (true) {
    if (canBus.read(msg)) {
      Serial.print("Received CAN ID: 0x");
      Serial.print(msg.id, HEX);
      Serial.print(" | Length: ");
      Serial.print(msg.len);
      Serial.print(" | Data: ");

      for (int i = 0; i < msg.len; i++) {
        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Debugging: Print message type
      if (msg.flags.extended) {
        Serial.println("Extended ID detected.");
      } else {
        Serial.println("Standard ID detected.");
      }
    } else {
      Serial.println("No messages received.");
    }
    delay(500);
  }
}
