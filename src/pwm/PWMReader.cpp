#include "PWMReader.h"
#include <Arduino.h>

PWMReader::PWMReader(int pinA, int pinB) : _pinA(pinA), _pinB(pinB) {}

void PWMReader::begin() {
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
}

unsigned long PWMReader::readPulseA() {
    return pulseIn(_pinA, HIGH, 25000); // Timeout set to 25ms
}

unsigned long PWMReader::readPulseB() {
    return pulseIn(_pinB, HIGH, 25000); // Timeout set to 25ms
}

