
#pragma once

#include <FlexCAN_T4.h>

class CANManager {
public:
  static FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &getInstance() {
    static FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canBus; // Single instance
    return canBus;
  }

  static void initialize() {
    static bool initialized = false;
    if (!initialized) {
      FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> &canBus = getInstance();

      // Explicitly set the clock source (Ensure Teensy CAN clock is correct)
      canBus.setClock(CLK_60MHz); // Teensy 4.x default CAN clock

      // Set baud rate correctly
      canBus.begin();
      canBus.setBaudRate(1000000); // Set to 1 Mbps (Spark MAX default)

      Serial.println("CAN Bus Initialized with 60MHz clock.");
      initialized = true; // Prevent re-initialization
    }
  }
};
