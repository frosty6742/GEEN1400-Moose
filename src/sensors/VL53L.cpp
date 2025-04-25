#include "VL53L.hpp"
#include <Arduino.h>
#include "WireIMXRT.h"
#include "usb_serial.h"
#include "vl53l1x-arduino-master/VL53L1X.h"

VL53L1X tofD;

VL53L::VL53L() {}

void VL53L::init() {
  //tofD.init()
  //tofD.setBus(&Wire);
  delay(200);
  if (!tofD.init()) {
    Serial.println("Failed to detect VL53L1X sensor!");
  } else {
    Serial.println("VL53L1X TOF sensor initalized.");
  }

  tofD.setTimeout(500);
  tofD.startContinuous(50);
}


float VL53L::getDistance() {
  return tofD.read();
}
