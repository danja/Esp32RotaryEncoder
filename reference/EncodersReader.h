#ifndef _ENCODERREADER_H
#define _ENCODERREADER_H

#include <Dispatcher.hpp>
#include <Esp32RotaryEncoder.h>

enum ButtonEventEnum
{
    BUTTON_1_EVENT,
    BUTTON_2_EVENT
};

enum EncoderEventEnum
{
    ENCODER_1_EVENT,
    ENCODER_2_EVENT
};

class EncodersReader
{
public:
    EncodersReader();
    void setScale1(long minValue, long maxValue, long steps, boolean invert, bool circleValues);
    void setScale2(long minValue, long maxValue, long steps, boolean invert, bool circleValues);

    void operate();

    Dispatcher<ButtonEventEnum> buttonDispatcher;
    Dispatcher<EncoderEventEnum, float> encoderDispatcher;

private:
};

#endif