#include "FlightController.hpp"
#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "VL53L.hpp"
#include "WireIMXRT.h"
#include "core_pins.h"
#include "pwm/SparkMaxPWM.h"
#include "usb_serial.h"

FlightController::FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR,
                                   VL53L &tofD, BNO085 &bnoL, AS7341 &clrSensLD,
                                   AS7341 &clrSensRD)
    : motorL(motorL), motorR(motorR), tofD(tofD), bnoL(bnoL),
      clrSensLD(clrSensLD), clrSensRD(clrSensRD), currentMode(LINE_FOLLOW) {}

void FlightController::set_control_mode(ControlMode mode) {
  Serial.print("Control Mode set to: ");
  Serial.println(mode);
  currentMode = mode;
}

void FlightController::update() {
  switch (currentMode) {
  case LINE_FOLLOW:
    lineFollow();
    break;
  case MANUAL:
    manual();
    break;
  case STOP:
    stop();
    break;
  case TEST:
    test();
    break;
  }
}

void FlightController::init() {
  Wire.begin();
  Wire1.begin();
  motorL.begin();
  motorR.begin();
  tofD.init();
  bnoL.begin();
  clrSensRD.begin(Wire);
  clrSensLD.begin(Wire1);

  Serial.println("Flight Controller Started.");

  delay(1000);
}

// Mode 1: lineFollow
void FlightController::lineFollow() {

  //float dist = tofD.getDistance();
  float dist = 50;
  const int threshold = 200; // Tune this based on your lighting & surface

  bool leftOnLine = clrSensLD.detectLine();
  bool rightOnLine = clrSensRD.detectLine();


  if (dist < 20 && dist > 5) { // Turn
    motorL.setSpeed(-0.1);
    motorL.setSpeed(0.1);
    delay(2000);
  } else if (dist < 5) {
    motorL.setSpeed(-0.0);
    motorR.setSpeed(0.0);
  } else {
    //call line follow method here
    if (leftOnLine && !rightOnLine) {
      // Veering right, so turn left
      motorL.setSpeed(-0.18);
      motorR.setSpeed(0.2);
    } else if (!leftOnLine && rightOnLine) {
      // Veering left, so turn right
      motorL.setSpeed(-0.2);
      motorR.setSpeed(0.18);
    } else if (leftOnLine && rightOnLine) {
      // On line
      motorL.setSpeed(-0.1);
      motorR.setSpeed(0.1);
    } else {
      // Lost line, stop or search
     motorL.setSpeed(0.0);
     motorR.setSpeed(0.0);
    }

  }
}


// Mode 2: manual drive
void FlightController::manual() {}

void FlightController::stop() {}



void FlightController::test() {
  Serial.println("Testing Sensors");

  //float dist = tofD.getDistance();
  //Serial.println(dist);

  // bnoDataL = bnoL.getIMUData();
  //bnoL.printIMUData();

  //clrSensRD.printData();
  Serial.println("LD");
  bool detected = clrSensLD.detectLine();
  Serial.println(detected);

  Serial.println("RD");
  bool detectedR = clrSensRD.detectLine();
  Serial.println(detectedR);
  //motorL.setSpeed(-0.07);
  //motorR.setSpeed(0.1);
}


/*
void FlightController::test() {
  double baseSpeed = 0.1;
  double correctionGain = 0.5;
  static bool initialized = false;
  static float targetYaw = 0;

  IMUData bnoDataL;
  bnoDataL = bnoL.getIMUData();

  // Initialize target yaw once
  if (!initialized) {
    bnoL.update();  // Make sure to call update if needed before reading
    
    targetYaw = bnoDataL.yE; // Assume getYaw() returns yaw in degrees
    initialized = true;
  }

  bnoL.update();  // Continuously update IMU
  float currentYaw = bnoDataL.yE;
  float yawError = targetYaw - currentYaw;

  // Normalize error to [-180, 180]
  if (yawError > 180) yawError -= 360;
  if (yawError < -180) yawError += 360;

  float correction = correctionGain * yawError;

  float leftSpeed = baseSpeed - correction;
  float rightSpeed = baseSpeed + correction;

  // Constrain speeds to valid range [-1.0, 1.0]
  leftSpeed = constrain(leftSpeed, -1.0, 1.0);
  rightSpeed = constrain(rightSpeed, -1.0, 1.0);

  motorL.setSpeed(leftSpeed);
  motorR.setSpeed(rightSpeed);
}
*/
