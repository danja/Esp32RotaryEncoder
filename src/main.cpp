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

void setup()
{
    Serial.begin(115200);
    // setScale(float minValue, float maxValue, float stepSize, bool invert, bool circleValues)
    encoder1.setScale(0, 10, 0.1, false, true);
}

// position of each rotary encoder
// int position1 = 0;
// int position2 = 0;

volatile bool RotaryEncoder::moved = false;

void handleChange1(float value)
{
    Serial.print("A : ");
    Serial.println(value);
}

void handleChange2(float value)
{
    Serial.print("B : ");
    Serial.println(value);
}

void loop()
{
    if (RotaryEncoder::moved)
    {
        int8_t rotationValue1 = encoder1.read();
        int8_t rotationValue2 = encoder2.read();

        // If valid movement, do something
        if (rotationValue1 != 0)
        {
            handleChange1(encoder1.value());
        }

        // If valid movement, do something
        if (rotationValue2 != 0)
        {
            handleChange2(encoder2.value());
        }
    }

    if (digitalRead(ENCODER_1_SW) == LOW)
    {
        // position1 = 0;
        Serial.print("X");
        //     Serial.println(position1);
    }
}
