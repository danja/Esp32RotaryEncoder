#include <Arduino.h>
#include <RotaryEncoder.h>

// TODO rename
// Interrupt routine just sets a flag when rotation is detected
void IRAM_ATTR RotaryEncoder::rotary()
{
	rotaryEncoder = true;
}

RotaryEncoder::RotaryEncoder(int gpioCLK, int gpioDT, int gpioSW)
{
	this->gpioCLK = gpioCLK;
	this->gpioDT = gpioDT;
	this->gpioSW = gpioSW;

	pinMode(this->gpioCLK, INPUT_PULLDOWN);
	pinMode(this->gpioDT, INPUT_PULLDOWN);
	pinMode(this->gpioSW, INPUT_PULLDOWN);

	attachInterrupt(digitalPinToInterrupt(gpioCLK), RotaryEncoder::rotary, CHANGE);
	attachInterrupt(digitalPinToInterrupt(gpioDT), RotaryEncoder::rotary, CHANGE);
}

void RotaryEncoder::setScale(long minValue, long maxValue, long steps, bool invert, bool circleValues)
{
	this->steps = steps;
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->invert = invert;
	this->circleValues = circleValues;
}

// Rotary encoder has moved (interrupt tells us) but what happened?
// See https://www.pinteric.com/rotary.html
int RotaryEncoder::checkRotaryEncoder()
{
	// Reset the flag that brought us here (from ISR)
	rotaryEncoder = false;

	static uint8_t lrmem = 3;
	static int lrsum = 0;
	static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

	// Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
	int8_t l = digitalRead(this->gpioCLK);
	int8_t r = digitalRead(this->gpioDT);

	// Move previous value 2 bits to the left and add in our new values
	lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

	// Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
	lrsum += TRANS[lrmem];

	/* encoder not in the neutral (detent) state */
	if (lrsum % 4 != 0)
	{
		return 0;
	}

	/* encoder in the neutral state - clockwise rotation*/
	if (lrsum == 4)
	{
		lrsum = 0;
		return 1;
	}

	/* encoder in the neutral state - anti-clockwise rotation*/
	if (lrsum == -4)
	{
		lrsum = 0;
		return -1;
	}

	// An impossible rotation has been detected - ignore the movement
	lrsum = 0;
	return 0;
}
