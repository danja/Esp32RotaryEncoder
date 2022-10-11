#ifndef _ROTARYENCODER_H
#define _ROTARYENCODER_H

#include <Arduino.h>

class RotaryEncoder
{
public:
    RotaryEncoder(int gpioCLK, int gpioDT, int gpioSW);
    //  void init(int gpioCLK, int gpioDT, int gpioSW);
    void setScale(long minValue, long maxValue, long steps, boolean invert, bool circleValues);
    void operate();

    // TODO rename
    // Interrupt routine just sets a flag when rotation is detected
    static void IRAM_ATTR rotary();

    int checkRotaryEncoder();
    // Flag from interrupt routine (moved=true)

    static volatile bool rotaryEncoder; //

private:
    int gpioCLK;
    int gpioDT;
    int gpioSW;

    long steps = 100;
    long minValue = 0;
    long maxValue = 100;
    bool circleValues = false;
    bool invert = false;
};

#endif