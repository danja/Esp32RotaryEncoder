#include <Arduino.h>
#include <RotaryEncoder.h>

// GPIO ports for rotary encoders

const int ENCODER_1_CLK = 34;
const int ENCODER_1_DT = 35;
const int ENCODER_1_SW = 4;

const int ENCODER_2_CLK = 36;
const int ENCODER_2_DT = 39;
const int ENCODER_2_SW = 16;

RotaryEncoder encoder1;
RotaryEncoder encoder2;
// RotaryEncoder encoder1 = RotaryEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);

void setup()
{
    Serial.begin(115200);
    encoder1.init(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
    encoder2.init(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
    encoder1.setScale(10, 20, 0.1, false, true);
}

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
