#ifndef PWMREADER_H
#define PWMREADER_H

class PWMReader {
public:
    PWMReader(int pinA, int pinB);
    void begin();
    unsigned long readPulseA();
    unsigned long readPulseB();

private:
    int _pinA;
    int _pinB;
};

#endif

