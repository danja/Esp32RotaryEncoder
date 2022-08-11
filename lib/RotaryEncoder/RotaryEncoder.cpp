#include <RotaryEncoder.h>

// RotaryEncoder::RotaryEncoder() {}

RotaryEncoder::RotaryEncoder(const uint8_t clk, const uint8_t dt, const uint8_t sw, const uint8_t steps)
{
    encoder = Esp32RotaryEncoder(clk, dt, sw, -1, steps);
    pinMode(clk, INPUT);
    pinMode(dt, INPUT);
    pinMode(sw, INPUT);
}

// void Esp32RotaryEncoder::setup(void (*ISR_callback)(void), void (*ISR_button)(void))
void RotaryEncoder::setup(void (*ISR_callback)(void), void (*ISR_button)(void))
{
    encoder.setup(ISR_callback, ISR_button);
    encoder.begin();
    encoder.setBoundaries(0, 100, false);
}

boolean RotaryEncoder::changed()
{
    return encoder.encoderChanged();
}

uint8_t RotaryEncoder::getValue()
{
    return encoder.readEncoder();
}

void IRAM_ATTR RotaryEncoder::readEncoder_ISR()
{
    encoder.readEncoder_ISR();
}

void IRAM_ATTR RotaryEncoder::readButton_ISR()
{
    encoder.readButton_ISR();
}
