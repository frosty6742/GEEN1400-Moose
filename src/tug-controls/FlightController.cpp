#include "FlightController.hpp"
#include "AS7341_ColorSens.hpp"
#include "BNO085.hpp"
#include "HardwareSerial.h"
#include "VL53L.hpp"
#include "WireIMXRT.h"
#include "core_pins.h"
#include "pwm/PWMReader.h"
#include "pwm/SparkMaxPWM.h"
#include "usb_serial.h"
#include "wiring.h"
#include <Arduino.h>
#include <chrono>
#include <ios>

FlightController::FlightController(SparkMaxPWM &motorL, SparkMaxPWM &motorR,
                                   VL53L &tofD, BNO085 &bnoL, AS7341 &clrSensLD,
                                   AS7341 &clrSensRD, PWMReader pwmReader)
    : motorL(motorL), motorR(motorR), tofD(tofD), bnoL(bnoL),
      clrSensLD(clrSensLD), clrSensRD(clrSensRD), pwmReader(pwmReader), currentMode(LINE_FOLLOW) {}

void FlightController::set_control_mode(ControlMode mode) {
  Serial.print("Control Mode set to: ");
  Serial.println(mode);
  currentMode = mode;
}

void FlightController::update() {
  switch (currentMode) {
  case LINE_FOLLOW:
    lineFollow();
    //Serial.println("LINE_FOLLOW");
    break;
  case MANUAL:
    manual();
    break;
  case STOP:
    stop();
    //Serial.println("STOP");
    break;
  case TEST:
    test();
    break;
  case RC:
    //Serial.println("RC");
    rc();
    break;
  }
}

void FlightController::init() {
  motorL.begin();
  motorR.begin();
  tofD.init();
  bnoL.begin();
  clrSensRD.begin(Wire1);
  clrSensLD.begin(Wire);

  Serial.println("Flight Controller Started.");

  delay(1000);
}
/*
// Mode 1: lineFollow
void FlightController::lineFollow() {

  //float dist = tofD.getDistance();
  float dist = 50;

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
    if (leftOnLine && !rightOnLine) {

      Serial.println("Veering right, so turn left");
      motorL.setSpeed(-0.08);
      motorR.setSpeed(0.1);
    } else if (!leftOnLine && rightOnLine) {
      Serial.println("Veering left, so turn right");
      motorL.setSpeed(-0.1);
      motorR.setSpeed(0.08);
    } else if (leftOnLine || rightOnLine) {
      Serial.println("On line (detected by either sensor)");
      motorL.setSpeed(-0.1);
      motorR.setSpeed(0.1);
    } else {
      Serial.println("Lost line, stop or search");
      motorL.setSpeed(0.0);
      motorR.setSpeed(0.0);
    }

  }
}
 

#include "LineTracker.hpp"

void FlightController::lineFollow() {
    //float dist = tofD.getDistance(); // Optional: enable if needed
    float dist = 50;  // Simulated/test value

    //clrSensLD.printData();
    //clrSensRD.printData();

    bool leftOnLine = clrSensLD.detectLine();
    bool rightOnLine = clrSensRD.detectLine();

    Serial.println(leftOnLine);
    Serial.println(rightOnLine);
  

    static LineTracker tracker;
    LinePosition position = tracker.detectPosition(leftOnLine, rightOnLine);

  
    if (dist < 20 && dist > 5) {
        // Turn around or slow turn
        motorL.setSpeed(0.05);
        motorR.setSpeed(0.05);
        delay(2000);
    } else if (dist <= 5) {
        // Obstacle very close, stop
        motorL.setSpeed(0.0);
        motorR.setSpeed(0.0);
    } else {
        // Line following behavior
        switch (position) {
            case LinePosition::UNDER_LEFT:
                // Veering right, turn left
                motorL.setSpeed(0.09);
                motorR.setSpeed(-0.1);
                break;

            case LinePosition::UNDER_RIGHT:
                // Veering left, turn right
                motorL.setSpeed(0.14);
                motorR.setSpeed(-0.08);
                break;

            case LinePosition::CENTERED:
                // Move straight forward
                motorL.setSpeed(0.11);
                motorR.setSpeed(-0.1);
                break;

            case LinePosition::OUTSIDE_LEFT:
                // Try to recover from overshooting left
                motorL.setSpeed(-0.0);
                motorR.setSpeed(0.00);
                break;

            case LinePosition::OUTSIDE_RIGHT:
                // Try to recover from overshooting right
                motorL.setSpeed(0.00);
                motorR.setSpeed(-0.0);
                break;

            case LinePosition::LOST:
            default:
                // Lost line, stop or implement search routine
                motorL.setSpeed(0.0);
                motorR.setSpeed(0.0);
                break;
        }
    }
}


#include "FlightController.hpp"
#include "LineFollowerPID.hpp"

void FlightController::lineFollow() {
    float dist = 50; // Simulated/test value

    bool leftOnLine = clrSensLD.detectLine();
    bool rightOnLine = clrSensRD.detectLine();

    Serial.println(leftOnLine);
    Serial.println(rightOnLine);

    static LineFollowerPID tracker(0.5, 0.0, 0.1); // Example PID values

    float dt = 0.02f; // Assume a 20ms loop time, adjust as necessary
    float control = tracker.update(leftOnLine, rightOnLine, dt);

    if (dist < 20 && dist > 5) {
        motorL.setSpeed(0.05);
        motorR.setSpeed(0.05);
        delay(2000);
    } else if (dist <= 5) {
        motorL.setSpeed(0.0);
        motorR.setSpeed(0.0);
    } else {
        // Control motors based on PID output
        float baseSpeed = 0.1f;
        float leftSpeed = baseSpeed - control;
        float rightSpeed = -baseSpeed - control;

        // Clamp speeds to valid range if necessary
        leftSpeed = constrain(leftSpeed, -1.0f, 1.0f);
        rightSpeed = constrain(rightSpeed, -1.0f, 1.0f);

        motorL.setSpeed(leftSpeed);
        motorR.setSpeed(rightSpeed);
    }
}

*/ 


void FlightController::lineFollow(){
    bool leftOnLine = clrSensLD.detectLine();
    bool rightOnLine = clrSensRD.detectLine();
  
  clrSensRD.printData();

  bool fellL;
  bool fellR;

  Serial.println(leftOnLine);
  Serial.println(rightOnLine);

  if(leftOnLine&&rightOnLine){
      fellL=false;
      fellR=false;
      motorL.setSpeed(0.14);
      motorR.setSpeed(-0.1);
      Serial.println("Online");
  } else if (leftOnLine&& !rightOnLine) {
      fellR=true;
      fellL=false;
      motorL.setSpeed(0.12);
      motorR.setSpeed(-0.1);
      Serial.print("YL, NR");
  } else if (!leftOnLine&& rightOnLine) {
    fellR=false;
    fellL=true;
    motorL.setSpeed(0.14);
    motorR.setSpeed(-0.08);
    Serial.println("NL, YR");
  } else if (!leftOnLine&& !rightOnLine) {
    if(fellR){
      motorL.setSpeed(0.12);
      motorR.setSpeed(-0.1);
    } else if (fellL) {
      motorL.setSpeed(0.14);
      motorR.setSpeed(-0.08);
    }
    motorL.setSpeed(0);
    motorR.setSpeed(0);
    Serial.println("NOL");
  }
    

  
} 




// Mode 2: manual drive
void FlightController::manual() {}

void FlightController::stop() {
  motorR.setSpeed(0);
  motorL.setSpeed(0);
}



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
/*
    const float minPulse = 988.0f;
    const float maxPulse = 2010.0f;
    const float midPulse = (minPulse + maxPulse) / 2.0f;
    const float maxDelta = 0.5f; // Max allowed difference between shaped inputs

void FlightController::rc() {
    unsigned long pulseA = pwmReader.readPulseA();
    unsigned long pulseB = pwmReader.readPulseB();

    float controlA = (pulseA - midPulse) / (maxPulse - midPulse);
    float controlB = (pulseB - midPulse) / (maxPulse - midPulse);

    // Clamp controlA to [-1, 1]
    if (controlA > 1.0f) controlA = 1.0f;
    else if (controlA < -1.0f) controlA = -1.0f;

    // Clamp controlB to [-1, 1]
    if (controlB > 1.0f) controlB = 1.0f;
    else if (controlB < -1.0f) controlB = -1.0f;

    // Apply x^2 while keeping the original sign
    float shapedA = controlA >= 0 ? controlA * controlA : -1 * controlA * controlA;
    float shapedB = controlB >= 0 ? controlB * controlB : -1 * controlB * controlB;

    // Limit the delta to reduce turning sharpness
    float delta = shapedA - shapedB;
    if (delta > maxDelta) {
        shapedA = shapedB + maxDelta;
    } else if (delta < -maxDelta) {
        shapedB = shapedA + maxDelta;
    }

  //Serial.println(pulseA);
  //Serial.println(pulseB);

    // Set motor speeds
    motorR.setSpeed(-0.75 * shapedB);
    motorL.setSpeed(0.75 * shapedA);

    //motorL.setSpeed(0.25);
    //motorR.setSpeed(-0.25);
  
}

*/
void FlightController::rc() {
    const float minPulse = 988.0f;
    const float maxPulse = 2010.0f;
    const float midPulse = (minPulse + maxPulse) / 2.0f;

    unsigned long pulseThrottle = pwmReader.readPulseA();  // Throttle input
    unsigned long pulseSteering = pwmReader.readPulseB();  // Steering input

    // Normalize throttle and steering to range [-1, 1]
    float throttle = (pulseThrottle - midPulse) / (maxPulse - midPulse);
    float steering = (pulseSteering - midPulse) / (maxPulse - midPulse);
    
    steering = steering * 0.6;


    // Apply shaping (square input while keeping sign)
    float shapedThrottle = (throttle >= 0 ? throttle * throttle : -throttle * throttle);
    float shapedSteering = (steering >= 0 ? steering * steering : -steering * steering);

    // Mix throttle and steering for differential drive
    float leftSpeed = shapedThrottle + shapedSteering;
    float rightSpeed = shapedThrottle - shapedSteering;

    // Scale motor speeds (adjust multiplier to your system)
    motorL.setSpeed(0.75f * leftSpeed);
    motorR.setSpeed(-0.75f * rightSpeed);
}

