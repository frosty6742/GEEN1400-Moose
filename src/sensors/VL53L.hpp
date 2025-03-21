#pragma once

#include <Arduino.h>
#include "vl53l1x-arduino-master/VL53L1X.h"

class VL53L {
public:
    VL53L();   // Constructor
    void init();
    float getDistance(); 
    void checkStatus();

private:
};


