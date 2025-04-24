#pragma once

#include "Adafruit_AS7341/Adafruit_AS7341.h"
#include <Arduino.h>

class AS7341 {
public:
  AS7341();
  void begin(TwoWire &wirePort); 
  void printData();
  bool detectLine();

private:
  Adafruit_AS7341 as7341D;
};
