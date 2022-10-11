#include <Arduino.h>
#include <Esp32RotaryEncoder.h>
#include <EncodersReader.h>

// GPIO ports for rotary encoders
const int ENCODER_1_CLK = 34;
const int ENCODER_1_DT = 35;
const int ENCODER_1_SW = 16;

const int ENCODER_2_CLK = 36;
const int ENCODER_2_DT = 39;
const int ENCODER_2_SW = 4;

const int ENCODER_1_STEPS = 10;
const int ENCODER_2_STEPS = 10;

Esp32RotaryEncoder encoder1 = Esp32RotaryEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW); // brightness
Esp32RotaryEncoder encoder2 = Esp32RotaryEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW); // everything else

// forward declarations
void IRAM_ATTR encoderEvent1();
void IRAM_ATTR buttonEvent1();
void IRAM_ATTR encoderEvent2();
void IRAM_ATTR buttonEvent2();

EncodersReader::EncodersReader()
{
    encoder1.setup(encoderEvent1, buttonEvent1); // pass the callback functions (leave as-is)
    encoder2.setup(encoderEvent2, buttonEvent2); // pass the callback functions (leave as-is)
}

void EncodersReader::operate()
{
    if (encoder1.buttonClicked()) // look at a flag
    {
        buttonDispatcher.broadcast(BUTTON_1_EVENT);
        encoder1.resetButton(); // reset the flag
    }

    if (encoder1.valueChanged()) // look at a flag
    {
        encoderDispatcher.broadcast(ENCODER_1_EVENT, encoder1.getValue());
        encoder1.resetValue(); // reset the flag
    }

    if (encoder2.buttonClicked()) // look at a flag
    {
        buttonDispatcher.broadcast(BUTTON_2_EVENT);
        encoder2.resetButton(); // reset the flag
    }

    if (encoder2.valueChanged()) // look at a flag
    {
        encoderDispatcher.broadcast(ENCODER_2_EVENT, encoder2.getValue());
        encoder2.resetValue(); // reset the flag
    }
}

void EncodersReader::setScale1(long minValue, long maxValue, long steps, boolean invert, bool circleValues)
{
    encoder1.setScale(minValue, maxValue, steps, invert, circleValues);
}

void EncodersReader::setScale2(long minValue, long maxValue, long steps, boolean invert, bool circleValues)
{
    encoder2.setScale(minValue, maxValue, steps, invert, circleValues);
}

void IRAM_ATTR encoderEvent1()
{
    encoder1.updateValue();
}

void IRAM_ATTR buttonEvent1()
{
    encoder1.readButton_ISR();
}

void IRAM_ATTR encoderEvent2()
{
    encoder2.updateValue();
}

void IRAM_ATTR buttonEvent2()
{
    encoder2.readButton_ISR();
}