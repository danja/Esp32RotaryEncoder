
#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include <Esp32RotaryEncoder.h>

class RotaryEncoder
{

public:
    RotaryEncoder(const uint8_t clk, const uint8_t dt, const uint8_t sw, const uint8_t steps);

    void setup(void (*ISR_callback)(void), void (*ISR_button)(void));
    void IRAM_ATTR readEncoder_ISR();
    void IRAM_ATTR readButton_ISR();

    boolean changed();
    uint8_t getValue();

private:
    Esp32RotaryEncoder encoder;

    void IRAM_ATTR readEncoder();
};

#endif