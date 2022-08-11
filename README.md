# Esp32RotaryEncoder

Minimal library for interfacing rotary encoders on the ESP32

### Target Devices

### Status

**2022-08-11** not ready yet

Largely derived from [ai-esp32-rotary-encoder](https://github.com/igorantolic/ai-esp32-rotary-encoder).

While working on [https://github.com/danja/dreamachine](Dreamachine) I started using the [ai-esp32-rotary-encoder](https://github.com/igorantolic/ai-esp32-rotary-encoder) but found that although it included quite sophisticated features, I didn't really need these. More significantly the general approach didn't work with what I wanted and so I've stripped it back and modified it to better fit my requirements.

- remove ESP8266 bits (for clarity)
- remove defaults
- use seperate encoder + button interrupts (setup( , ))
- remove current button handling
- add button debouncing
