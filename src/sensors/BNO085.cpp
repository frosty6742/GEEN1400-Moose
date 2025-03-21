// BNO085Interface.cpp
// Usefull info https://github.com/adafruit/Adafruit_BNO08x/blob/master/examples/more_reports/more_reports.ino
#include "BNO085.hpp"
#include "sh2.h"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <iterator>
#include <sh2_SensorValue.h>

BNO085::BNO085(int8_t resetPin) : bno085D(resetPin) {}

bool BNO085::begin() {
  Serial.println("Initializing BNO085...");

  if (!bno085D.begin_I2C()) {
    Serial.println("Failed to initialize BNO08x");
    return false;
  }
  Serial.println("BNO08x Initialized!");

  printProductInfo();

  bno085D.enableReport(SH2_ACCELEROMETER, 10000);
  bno085D.enableReport(SH2_GYROSCOPE_CALIBRATED, 10000);
  bno085D.enableReport(SH2_GAME_ROTATION_VECTOR, 10000);
  bno085D.enableReport(SH2_FLIP_DETECTOR, 10000);

  return true;
}

void BNO085::update() {
  delay(10);
  if (bno085D.wasReset()) {
    Serial.println("Sensor was reset. Reconfiguring reports...");
    //setReports();
  }
  if (bno085D.getSensorEvent(&sensorValue)) {
    processSensorEvent();
  }
}


void BNO085::printProductInfo() {
  for (int i = 0; i < bno085D.prodIds.numEntries; i++) {
    Serial.print("Part ");
    Serial.print(bno085D.prodIds.entry[i].swPartNumber);
    Serial.print(": Version ");
    Serial.print(bno085D.prodIds.entry[i].swVersionMajor);
    Serial.print(".");
    Serial.print(bno085D.prodIds.entry[i].swVersionMinor);
    Serial.print(".");
    Serial.print(bno085D.prodIds.entry[i].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno085D.prodIds.entry[i].swBuildNumber);
  }
}

/*
void BNO085::processSensorEvent() {
  switch (sensorValue.sensorId) {
  case SH2_GAME_ROTATION_VECTOR:
    Serial.print("Game Rotation Vector - r: ");
    Serial.print(sensorValue.un.gameRotationVector.real);
    Serial.print(" i: ");
    Serial.print(sensorValue.un.gameRotationVector.i);
    Serial.print(" j: ");
    Serial.print(sensorValue.un.gameRotationVector.j);
    Serial.print(" k: ");
    Serial.println(sensorValue.un.gameRotationVector.k);
    break;
  }
}
*/

IMUData BNO085::getIMUData(){
  IMUData data;
  bno085D.getSensorEvent(&sensorValue);
   
  data.xB = sensorValue.un.gyroscope.x;
  data.yB = sensorValue.un.gyroscope.y;
  data.zB = sensorValue.un.gyroscope.z;

  data.accelXB = sensorValue.un.accelerometer.x;
  data.accelYB = sensorValue.un.accelerometer.y;
  data.accelZB = sensorValue.un.accelerometer.z;

  data.xE = sensorValue.un.gameRotationVector.i;
  data.yE = sensorValue.un.gameRotationVector.j;
  data.zE = sensorValue.un.gameRotationVector.k;

  data.fliped = sensorValue.un.flipDetector.flip;

  return data;
}

void BNO085::printIMUData() {
    IMUData data = getIMUData();

    Serial.print("Gyroscope: ");
    Serial.print("X: "); Serial.print(data.xB);
    Serial.print(" Y: "); Serial.print(data.yB);
    Serial.print(" Z: "); Serial.println(data.zB);

    Serial.print("Accelerometer: ");
    Serial.print("X: "); Serial.print(data.accelXB);
    Serial.print(" Y: "); Serial.print(data.accelYB);
    Serial.print(" Z: "); Serial.println(data.accelZB);

    Serial.print("Earth Rotation Vector: ");
    Serial.print("i: "); Serial.print(data.xE);
    Serial.print(" j: "); Serial.print(data.yE);
    Serial.print(" k: "); Serial.println(data.zE);

    Serial.print("Flip Detector: ");
    Serial.println(data.fliped ? "Flipped" : "Not Flipped");
}


