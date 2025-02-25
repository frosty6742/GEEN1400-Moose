#include <Arduino.h>

#include "git_info.h"

#include <TeensyDebug.h>

// Loop constants
#define LOOP_FREQ 1000
#define HEARTBEAT_FREQ 2

// Declare global objects
LEDBoard led;

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
  uint32_t loopc = 0; // Loop counter for heartbeat

  Serial.begin(115200); // the serial monitor is actually always active (for
                        // debug use Serial.println & tycmd)
  debug.begin(SerialUSB1);

  print_logo();

  // Execute setup functions
  pinMode(LED_BUILTIN, OUTPUT);

  led.init();
  // main loop timers
  Timer loop_timer;

  Serial.println("Entering main loop...\n");

  // Main loop
  while (true) {
    // start main loop time timer
    stall_timer.start();

    // Keep the loop running at the desired rate
    loop_timer.delay_micros((int)(1E6 / (float)(LOOP_FREQ)));
  }

  return 0;
}
