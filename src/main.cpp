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
}

// A turn counter for the rotary encoder (negative = anti-clockwise)
int rotationCounter = 200;

volatile bool RotaryEncoder::rotaryEncoder = false;

void loop()
{
    // Has rotary encoder moved?
    if (RotaryEncoder::rotaryEncoder) // RotaryEncoder::rotaryEncoder
    {
        // Get the movement (if valid)
        int8_t rotationValue = encoder1.checkRotaryEncoder();

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter += rotationValue * 5;
            Serial.print(rotationValue < 1 ? "L" : "R");
            Serial.println(rotationCounter);
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
