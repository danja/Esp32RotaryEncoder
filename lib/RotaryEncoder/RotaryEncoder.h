#ifndef _ROTARYENCODER_H
#define _ROTARYENCODER_H

#include <Arduino.h>

class RotaryEncoder
{
public:
    RotaryEncoder();

    void init(int gpioCLK, int gpioDT, int gpioSW);

    portMUX_TYPE encoderMux = portMUX_INITIALIZER_UNLOCKED;
    portMUX_TYPE buttonMux = portMUX_INITIALIZER_UNLOCKED;

    void setScale(float startValue, float minValue, float maxValue, float stepSize, boolean invert, bool circleValues);
    // void operate();

    // Interrupt routine sets a flag when rotation is detected
    static void IRAM_ATTR onEncoderChange();

    // Interrupt routine sets a flag when button is pressed
    static void IRAM_ATTR onButtonClick();

    // rotate flag
    static volatile bool moved;
    // click flag
    static volatile bool clicked;

    // returns the raw movement
    int readEncoder();
    bool readButton();

    void setValue(float value);

    // returns the scaled value
    float value();

private:
    int gpioCLK;
    int gpioDT;
    int gpioSW;

    int lrmem = 3;
    int lrsum = 0;

    long timer = 0;

    // int move = 0;
    long position = 0;

    float stepSize = 1;
    int nSteps = 0;
    float minValue = 0;
    float maxValue = 100;
    bool circleValues = false;
    bool invert = false;
};

#endif