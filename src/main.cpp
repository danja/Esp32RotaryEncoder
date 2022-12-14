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

void setup()
{
    Serial.begin(115200);
    encoder1.init(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
    encoder2.init(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);

    //  void configure(float startValue, float minValue, float maxValue, float stepSize, boolean invert, bool circleValues);
    encoder1.configure(1, 10, 20, 0.1, false, true);
}

void showConfig(RotaryEncoder encoder)
{
    Serial.print("minValue : ");
    Serial.println(encoder.minValue);
    Serial.print("maxValue : ");
    Serial.println(encoder.maxValue);
    Serial.print("stepSize : ");
    Serial.println(encoder.stepSize);
    Serial.print("invert : ");
    Serial.println(encoder.invert);
    Serial.print("circleValues : ");
    Serial.println(encoder.circleValues);
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
        showConfig(encoder1);
    }
    if (encoder2.readButton())
    {
        Serial.println("Button 2");
        showConfig(encoder2);
    }
}
