#include <Arduino.h>
#include <RotaryEncoder.h>

// GPIO ports for rotary encoders
const int ENCODER_1_CLK = 34;
const int ENCODER_1_DT = 35;
const int ENCODER_1_SW = 16;

const int ENCODER_2_CLK = 36;
const int ENCODER_2_DT = 39;
const int ENCODER_2_SW = 4;

RotaryEncoder encoder1 = RotaryEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
RotaryEncoder encoder2 = RotaryEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
// volatile bool RotaryEncoder::moved = false;
// volatile bool RotaryEncoder::clicked = false;

void setup()
{
    Serial.begin(115200);
    // setScale(float minValue, float maxValue, float stepSize, bool invert, bool circleValues)
    encoder1.setScale(0, 10, 0.1, false, true);
}

// position of each rotary encoder
// int position1 = 0;
// int position2 = 0;

void handleEncoder1(float value)
{
    Serial.print("A : ");
    Serial.println(value);
}

void handleEncoder2(float value)
{
    Serial.print("B : ");
    Serial.println(value);
}

void loop()
{
    if (RotaryEncoder::moved)
    {
        int8_t rotationValue1 = encoder1.readEncoder();
        int8_t rotationValue2 = encoder2.readEncoder();

        // If valid movement, do something
        if (rotationValue1 != 0)
        {
            handleEncoder1(encoder1.value());
        }

        // If valid movement, do something
        if (rotationValue2 != 0)
        {
            handleEncoder2(encoder2.value());
        }
    }

    if (encoder1.readButton())
    {
        Serial.println("Button 1");
    }
    if (encoder2.readButton())
    {
        Serial.println("Button 2");
    }
}
