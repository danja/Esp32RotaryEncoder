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
    // encoder1.init(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
    // encoder2.init(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);

    Serial.begin(115200);
}

// A turn counter for the rotary encoder (negative = anti-clockwise)
int rotationCounter = 200;

volatile bool RotaryEncoder::moved = false;

void handleChange1(long value)
{
    rotationCounter += value;
    Serial.print("A : ");
    Serial.print(value < 1 ? "L" : "R");
    Serial.println(rotationCounter);
}

void handleChange2(long value)
{
    rotationCounter += value;
    Serial.print("B : ");
    Serial.print(value < 1 ? "L" : "R");
    Serial.println(rotationCounter);
}

void loop()
{
    //  delay(100);

    // Has rotary encoder moved?
    if (RotaryEncoder::moved) // RotaryEncoder::moved
    {
        int8_t rotationValue = encoder1.read();
        int8_t rotationValue2 = encoder2.read();

        // If valid movement, do something
        if (rotationValue != 0)
        {
            handleChange1(rotationValue);
            //   RotaryEncoder::moved = false;
        }

        // If valid movement, do something
        if (rotationValue2 != 0)
        {
            handleChange2(rotationValue2);
            // RotaryEncoder::moved = false;
        }
    }

    if (digitalRead(ENCODER_1_SW) == LOW)
    {
        rotationCounter = 0;
        Serial.print("X");
        Serial.println(rotationCounter);

        // Wait until button released (demo only! Blocking call!)
        while (digitalRead(ENCODER_1_SW) == LOW)
        {
            delay(100);
        }
    }
}
