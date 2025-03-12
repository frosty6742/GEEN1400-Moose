#include <Arduino.h>

#include "git_info.h"
#include "tug-controls/SparkMaxESC.hpp"
#include "usb_serial.h"
#include <FlexCAN_T4.h>
#include <TeensyDebug.h>

// Loop constants
#define LOOP_FREQ 1000
#define HEARTBEAT_FREQ 2

// Declare global objects
SparkMaxESC motor(1);
// MotorController mControl(1, 2); // Left ESC ID = 1, Right ESC ID = 2

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

  Serial.println("Initializing Motor Controller...");
  motor.begin();

  Serial.println("Entering main loop...\n");

  delay(5000);
  // Main loop
  while (true) {
    Serial.println(motor.getMotorTemperature());
    //  mControl.getLeftTemperature();
    //   mControl.setMotorSpeed(0.2, 0.2); // INRANGE -1, 1
    //   mControl.printAllReadings();
  }

  return 0;
}
