# Esp32RotaryEncoder

Minimal library for interfacing rotary encoders on the ESP32

### Status

**2022-08-11** basically working but needs tweaks, have to try in use

### Target Devices

The specific encoders I got have built-in push-button, and came mounted on a small PCB, looking like this :

![encoder](https://github.com/danja/Esp32RotaryEncoder/blob/main/images/encoder.png)

I got mine very cheap from AZDelivery on Amazon. Other similar encoders should work with this library, but the values given by `setMode(...)` (TODO) may need changing (the PCB has pullup/pulldown? resistors).

### Usage

Changes to inputs on the ESP32 can be used to trigger interrupts, but because these block the whole system only very minimal code can be run without causing a crash. Here that code just sets flags and updates the encoder value.

For dev, this repo is currently set up as a PlatformIO (on VSCode on Ubuntu) project, the library code itself is in [Esp32RotaryEncoder.h](https://github.com/danja/Esp32RotaryEncoder/blob/main/lib/Esp32RotaryEncoder/Esp32RotaryEncoder.h) and [Esp32RotaryEncoder.cpp](https://github.com/danja/Esp32RotaryEncoder/blob/main/lib/Esp32RotaryEncoder/Esp32RotaryEncoder.cpp)

Core operation is:

```
void setup()
{
  Serial.begin(115200);
  encoder.setup(encoderEvent, buttonEvent); // pass the callback functions (leave as-is)
}

void loop()
{
  if (encoder.buttonClicked()) // look at a flag
  {
    Serial.println("click!"); // do stuff
    encoder.resetButton();    // reset the flag
  }

  if (encoder.valueChanged()) // look at a flag
  {
    Serial.print("Value :"); // do stuff
    Serial.println(encoder.getValue());
    encoder.resetValue(); // reset the flag
  }
}
```

This is used in the little demo in [main.cpp](https://github.com/danja/Esp32RotaryEncoder/blob/main/src/main.cpp)

Note that the button is **not debounced**. If the operations called on a click of the button are very short-lived, multiple presses may be detected (and debouncing will be needed).

### TODO

- remove acceleration
- add mode() function for optional pullup on GPIO inputs
- add (minimal) value range setup

### Origins

Largely derived from [ai-esp32-rotary-encoder](https://github.com/igorantolic/ai-esp32-rotary-encoder).

While working on [Dreamachine](https://github.com/danja/dreamachine) I started using the [ai-esp32-rotary-encoder](https://github.com/igorantolic/ai-esp32-rotary-encoder) but found that although it included quite sophisticated features, I didn't really need them. More significantly the general approach didn't work the way I wanted and so I've stripped it back and modified it to better fit my requirements.

For cleaner operation:

- removed current button handling
- remove acceleration (TODO)

For ease of (my) code reading :

- removed ESP8266 bits
- removed defaults

For ease of use in (my) application :

- used seperate encoder + button interrupts, in : setup(encoderCallback, buttonCallback)
- changed to flag-orientation

See also : https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
