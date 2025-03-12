#pragma once

#include <FlexCAN_T4.h>

class CANManager {
public:
    static FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>& getInstance() {
        static FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canBus;  // Single instance
        return canBus;
    }

    static void initialize() {
        static bool initialized = false;
        if (!initialized) {
            FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>& canBus = getInstance();
            canBus.begin();
            canBus.setBaudRate(1000000);
            Serial.println("CAN Bus Initialized.");
            initialized = true;  // Prevent re-initialization
        }
    }
};


