#include <Esp32RotaryEncoder.h>
#include <RotaryEncoder.h>

// GPIO ports for rotary encoders
const int ENCODER_CLK = 34;
const int ENCODER_DT = 35;
const int ENCODER_SW = 16;

const int ENCODER_STEPS = 10;

RotaryEncoder encoder = RotaryEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_SW, ENCODER_STEPS);

// forward declarations
void IRAM_ATTR readEncoderISR();
void IRAM_ATTR readButtonISR();

void setup()
{
  Serial.begin(115200);

  encoder.setup(readEncoderISR, readButtonISR);
}

void loop()
{

  if (encoder.changed())
  {
    Serial.print("Selected value is ");
    Serial.println(encoder.getValue(), 1);
  }
}

void IRAM_ATTR readEncoderISR()
{
  encoder.readEncoder_ISR();
  Serial.print("interrupt "); //// anything in here has to happen really fast
  Serial.println(encoder.getValue(), 1);
}

void IRAM_ATTR readButtonISR()
{
  encoder.readButton_ISR();
  Serial.print("button interrupt "); //// anything in here has to happen really fast
                                     // Serial.println(encoder2.getValue(), 1);
}
