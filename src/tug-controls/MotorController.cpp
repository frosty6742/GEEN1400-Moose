#include "MotorController.hpp"
#include <cmath>

MotorController::MotorController(uint8_t leftMotorID, uint8_t rightMotorID, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16>& canBus)
    : leftMotor(leftMotorID, canBus), rightMotor(rightMotorID, canBus) {}

void MotorController::begin() {
    leftMotor.begin();
    rightMotor.begin();
}

void MotorController::setThrottle(float throttle) {
    leftMotor.setPower(throttle);
    rightMotor.setPower(throttle);
}

void MotorController::setSteering(float steering) {
    leftMotor.setPower(steering);
    rightMotor.setPower(-steering);
}

void MotorController::setThrottleAndSteering(float throttle, float steering) {
    float leftPower = throttle + steering;
    float rightPower = throttle - steering;

    // Clamp values between -1 and 1
    leftPower = fmaxf(-1.0f, fminf(1.0f, leftPower));
    rightPower = fmaxf(-1.0f, fminf(1.0f, rightPower));

    leftMotor.setPower(leftPower);
    rightMotor.setPower(rightPower);
}

void MotorController::emergencyStop() {
    leftMotor.stop();
    rightMotor.stop();
}

// Sensor Readings
float MotorController::getLeftEncoderPosition() { return leftMotor.getEncoderPosition(); }
float MotorController::getRightEncoderPosition() { return rightMotor.getEncoderPosition(); }
float MotorController::getLeftVelocity() { return leftMotor.getEncoderVelocity(); }
float MotorController::getRightVelocity() { return rightMotor.getEncoderVelocity(); }
float MotorController::getLeftCurrent() { return leftMotor.getMotorCurrent(); }
float MotorController::getRightCurrent() { return rightMotor.getMotorCurrent(); }
float MotorController::getLeftTemperature() { return leftMotor.getMotorTemperature(); }
float MotorController::getRightTemperature() { return rightMotor.getMotorTemperature(); }
uint16_t MotorController::getLeftFaults() { return leftMotor.getFaults(); }
uint16_t MotorController::getRightFaults() { return rightMotor.getFaults(); }

