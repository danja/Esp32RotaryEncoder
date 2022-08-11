#include <Esp32RotaryEncoder.h>

// GPIO ports for rotary encoders
const int ENCODER_CLK = 34;
const int ENCODER_DT = 35;
const int ENCODER_SW = 16;

const int ENCODER_STEPS = 10;

Esp32RotaryEncoder encoder = Esp32RotaryEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_SW);

// forward declarations
void IRAM_ATTR encoderEvent();
void IRAM_ATTR buttonEvent();

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

//// I've not found a better way of passing callbacks to the interrupt functions
//// modify at your own risk!
void IRAM_ATTR encoderEvent()
{
  encoder.updateValue();
}

void IRAM_ATTR buttonEvent()
{
  encoder.readButton_ISR();
}
