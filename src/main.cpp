#include <Arduino.h>

#include "FlightController.hpp"
#include "core_pins.h"
#include "git_info.h"
#include "pwm/SparkMaxPWM.h"
#include "usb_serial.h"
#include <TeensyDebug.h>

// Loop constants
#define LOOP_FREQ 1000
#define HEARTBEAT_FREQ 2

// Declare global objects
SparkMaxPWM motorL(1);
SparkMaxPWM motorR(2);

FlightController flightController(motorL, motorR);

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

  Serial.println("Initializing Flight Controller...");
  flightController.init();

  Serial.println("Entering main loop...\n");
  delay(1000);

  flightController.set_control_mode(LINE_FOLLOW);
  //  Main loop
  while (true) {
    flightController.update();
  }

  return 0;
}
