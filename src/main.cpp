#include <Arduino.h>

#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "FlightController.hpp"
#include "VL53L.hpp"
#include "core_pins.h"
#include "git_info.h"
#include "pwm/SparkMaxPWM.h"
#include "usb_serial.h"
#include <TeensyDebug.h>

// Loop constants
#define LOOP_FREQ 1000
#define HEARTBEAT_FREQ 2

// Declare global objects
// Motors
SparkMaxPWM motorL(8);
SparkMaxPWM motorR(10);

// Sensors
VL53L tof;
BNO085 bno;
AS7341 clrSensL;
AS7341 clrSensR;

FlightController flightController(motorL, motorR, tof, bno, clrSensL, clrSensR);

// DONT put anything else in this function. It is not a setup function
void print_logo() {
  if (Serial) {
    Serial.print("\033[0m");
    Serial.println("\n\033[1;92mFW Ver. 2.1.0");
    Serial.printf("\nLast Built: %s at %s", __DATE__, __TIME__);
    Serial.printf("\nGit Hash: %s", GIT_COMMIT_HASH);
    Serial.printf("\nGit Branch: %s", GIT_BRANCH);
    Serial.printf("\nCommit Message: %s", GIT_COMMIT_MSG);
    Serial.printf("\nRandom Num: %x", ARM_DWT_CYCCNT);
    Serial.println("\033[0m\n");
  }
}



void i2cScan() {
    Wire.begin(); // Join I2C bus as master
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to initialize (for Teensy)
  
  Serial.println("I2C Scanner for Teensy 4.1");
  
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Scan complete\n");

  delay(5000); // Wait 5 seconds before next scan
}

// Master loop
int main() {
  Serial.begin(115200); // the serial monitor is actually always active (for
                        // debug use Serial.println & tycmd)
  Serial.println("Serial Started");
  debug.begin(SerialUSB1);
  print_logo();

  i2cScan();

  Serial.println("Starting Flight Controller...");
  flightController.init();

  flightController.set_control_mode(LINE_FOLLOW);

  Serial.println("Entering main loop...\n");
  delay(1000);

  //  Main loop
  while (true) {
    flightController.update();
  }

  return 0;
}
