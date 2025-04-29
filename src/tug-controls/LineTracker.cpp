#include "LineTracker.hpp"
#include <Arduino.h>

LineTracker::LineTracker() { lastPosition = LinePosition::CENTERED; }

LinePosition LineTracker::detectPosition(bool leftOnLine, bool rightOnLine) {
  LinePosition currentPosition;

  if (leftOnLine && rightOnLine) {
    currentPosition = LinePosition::CENTERED;
    Serial.println("CENTERED");
  } else if (leftOnLine && !rightOnLine) {
    currentPosition = LinePosition::UNDER_LEFT;
    Serial.println("ULeft");
  } else if (!leftOnLine && rightOnLine) {
    currentPosition = LinePosition::UNDER_RIGHT;
    Serial.println("URight");
  } else {
    // Infer based on last known position
    if (lastPosition == LinePosition::UNDER_LEFT ||
        lastPosition == LinePosition::CENTERED) {
      currentPosition = LinePosition::OUTSIDE_RIGHT;
      Serial.println("OutRight");
    } else if (lastPosition == LinePosition::UNDER_RIGHT) {
      currentPosition = LinePosition::OUTSIDE_LEFT;
      Serial.println("OutLeft");
    } else {
      currentPosition = LinePosition::LOST;
      Serial.println("Lost");
    }
  }

  lastPosition = currentPosition;
  return currentPosition;
}

LinePosition LineTracker::getLastKnownPosition() const { return lastPosition; }
