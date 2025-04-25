// LineFollowerPID.cpp
#include "LineFollowerPID.hpp"

LineFollowerPID::LineFollowerPID(float kp, float ki, float kd)
    : kp_(kp), ki_(ki), kd_(kd), last_error_(0), integral_(0) {}

float LineFollowerPID::update(bool left_on_line, bool right_on_line, float dt) {
    float error = calculateError(left_on_line, right_on_line);

    // PID terms
    integral_ += error * dt;
    float derivative = (error - last_error_) / dt;

    float output = kp_ * error + ki_ * integral_ + kd_ * derivative;

    last_error_ = error;
    return output; 
}

float LineFollowerPID::calculateError(bool left_on_line, bool right_on_line) {
    if (left_on_line && !right_on_line) {
        return -0.05f; // Line detected left side -> steer left
    } else if (!left_on_line && right_on_line) {
        return 0.05f; // Line detected right side -> steer right
    } else if (left_on_line && right_on_line) {
        return 0.0f; // Line centered
    } else {
        // Neither sensor sees the line!
        // Use last known error or a fallback
        return last_error_;
    }
}

