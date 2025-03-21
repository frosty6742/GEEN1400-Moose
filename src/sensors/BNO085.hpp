#pragma once

#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <sh2_SensorValue.h>


struct IMUData{ 
float xB;
float yB;
float zB;
float accelXB;
float accelYB;
float accelZB;

float xE;
float yE;
float zE;

bool fliped;
};

class BNO085 {
public:
  BNO085(int8_t resetPin = -1);
  bool begin();
  void update();
  void printIMUData();
  IMUData getIMUData();

private:
  Adafruit_BNO08x bno085D;
  sh2_SensorValue_t sensorValue;
  void setReports();
  void printProductInfo();
  void processSensorEvent();
};
