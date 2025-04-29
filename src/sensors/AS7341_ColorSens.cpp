#include "AS7341_ColorSens.hpp"
#include "Adafruit_AS7341/Adafruit_AS7341.h"
#include "usb_serial.h"
#include <Arduino.h>

AS7341::AS7341() : as7341D() {}

void AS7341::begin(TwoWire &wirePort) {
  if (!as7341D.begin(0x39, &wirePort)) {
    Serial.println("Could not find AS7341");
  } else {
    Serial.println("AS7341 ClrSens initialized.");
  }

  as7341D.setATIME(10);
  as7341D.setASTEP(99);
  as7341D.setGain(AS7341_GAIN_256X);

  as7341D.enableLED(true);
}

void AS7341::printData() {
  // Read all channels at the same time and store in as7341 object
  if (!as7341D.readAllChannels()) {
    Serial.println("Error reading all channels!");
    return;
  }

  // Print out the stored values for each channel
  Serial.print("F1 415nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_415nm_F1));
  Serial.print("F2 445nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_445nm_F2));
  Serial.print("F3 480nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_480nm_F3));
  Serial.print("F4 515nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_515nm_F4));
  Serial.print("F5 555nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_555nm_F5));
  Serial.print("F6 590nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_590nm_F6));
  Serial.print("F7 630nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_630nm_F7));
  Serial.print("F8 680nm : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_680nm_F8));

  Serial.print("Clear    : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_CLEAR));

  Serial.print("Near IR  : ");
  Serial.println(as7341D.getChannel(AS7341_CHANNEL_NIR));
}

int AS7341::return590Nm() {
  return as7341D.getChannel(AS7341_CHANNEL_590nm_F6);
}

bool AS7341::detectLine() {

  if (!as7341D.readAllChannels()) {
    Serial.println("Error reading all channels!");
  }
  int reading = as7341D.getChannel(AS7341_CHANNEL_590nm_F6);
  Serial.println(reading);

  const int blackThreshold = 30; // values below this mean "black"

  return reading < blackThreshold;
}
