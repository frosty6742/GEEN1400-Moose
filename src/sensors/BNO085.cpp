// BNO085Interface.cpp
#include "BNO085.hpp"
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <sh2_SensorValue.h>

BNO085Interface::BNO085Interface(int8_t resetPin) : bno08x(resetPin) {}

bool BNO085Interface::begin() {
  Serial.println("Initializing BNO085...");

  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to initialize BNO08x");
    return false;
  }
  Serial.println("BNO08x Initialized!");

  printProductInfo();
  setReports();
  return true;
}

void BNO085Interface::update() {
  delay(10);
  if (bno08x.wasReset()) {
    Serial.println("Sensor was reset. Reconfiguring reports...");
    setReports();
  }
  if (bno08x.getSensorEvent(&sensorValue)) {
    processSensorEvent();
  }
}

void BNO085Interface::setReports() {
  Serial.println("Setting up sensor reports...");
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Failed to enable Game Rotation Vector");
  }
}

void BNO085Interface::printProductInfo() {
  for (int i = 0; i < bno08x.prodIds.numEntries; i++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[i].swPartNumber);
    Serial.print(": Version ");
    Serial.print(bno08x.prodIds.entry[i].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[i].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[i].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[i].swBuildNumber);
  }
}

void BNO085Interface::processSensorEvent() {
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
