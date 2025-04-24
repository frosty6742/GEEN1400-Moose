#include <Arduino.h>

#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "FlightController.hpp"
#include "VL53L.hpp"
#include "WireIMXRT.h"
#include "core_pins.h"
#include "git_info.h"
#include "pwm/PWMReader.h"
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

PWMReader pwmReader(14, 13);

FlightController flightController(motorL, motorR, tof, bno, clrSensL, clrSensR, pwmReader);

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




// Master loop
int main() {
  Serial.begin(115200); // the serial monitor is actually always active (for
                        // debug use Serial.println & tycmd)
  Serial.println("Serial Started");
  debug.begin(SerialUSB1);
  print_logo();


      // check to see if there is a crash report, and if so, print it repeatedly over Serial
    // in the future, we'll send this directly over comms
    if (CrashReport) {
        while (1) {
            Serial.println(CrashReport);
            Serial.println("\nReflash to clear CrashReport (and also please fix why it crashed)");
            delay(1000);
        }
    }


  Wire.begin();
  Wire1.begin();

  Serial.println("Starting Flight Controller...");
  flightController.init();

  flightController.set_control_mode(RC);

  Serial.println("Entering main loop...\n");
  delay(1000);

  //  Main loop
  while (true) {
    flightController.update();
  }

  return 0;
}
