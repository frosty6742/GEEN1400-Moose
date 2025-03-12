#pragma once

#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <sh2_SensorValue.h>

class BNO085Interface {
public:
  BNO085Interface(int8_t resetPin = -1);
  bool begin();
  void update();

private:
  Adafruit_BNO08x bno08x;
  sh2_SensorValue_t sensorValue;
  void setReports();
  void printProductInfo();
  void processSensorEvent();
};
