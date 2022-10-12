#include <Arduino.h>
#include <RotaryEncoder.h>

// Interrupt routine sets a flag when rotation is detected
void IRAM_ATTR RotaryEncoder::detect()
{
	RotaryEncoder::moved = true;
}

RotaryEncoder::RotaryEncoder(int gpioCLK, int gpioDT, int gpioSW)
{
	this->gpioCLK = gpioCLK;
	this->gpioDT = gpioDT;
	this->gpioSW = gpioSW;

	pinMode(this->gpioCLK, INPUT_PULLDOWN);
	pinMode(this->gpioDT, INPUT_PULLDOWN);
	pinMode(this->gpioSW, INPUT_PULLDOWN);

	attachInterrupt(digitalPinToInterrupt(gpioCLK), RotaryEncoder::detect, CHANGE);
	attachInterrupt(digitalPinToInterrupt(gpioDT), RotaryEncoder::detect, CHANGE);
}

void RotaryEncoder::setScale(float minValue, float maxValue, float stepSize, bool invert, bool circleValues)
{
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->stepSize = stepSize;
	this->invert = invert;
	this->circleValues = circleValues;
}

float RotaryEncoder::value()
{
	// move me
	int steps = (maxValue - minValue) / stepSize;

	if (circleValues)
	{
		if (position > steps)
		{
			position = 0;
		}
		if (position < 0)
		{
			position = steps;
		}
	}
	else
	{
		if (position > steps)
		{
			position = steps;
		}
		if (position < 0)
		{
			position = 0;
		}
	}

	float value = ((float)position) * stepSize + minValue;

	return value;
	// this->position;
}

// Rotary encoder has moved (interrupt tells us) but what happened?
// See https://www.pinteric.com/rotary.html
int RotaryEncoder::read()
{
	// Reset the flag that brought us here (from ISR)
	RotaryEncoder::moved = false;

	//	static uint8_t lrmem = 3;
	//	static int lrsum = 0;
	static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

	//	Serial.println("Reading ");
	//	Serial.println(this->gpioCLK);
	//	Serial.println(this->gpioDT);

	// Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
	int8_t l = digitalRead(this->gpioCLK);
	int8_t r = digitalRead(this->gpioDT);

	// Move previous value 2 bits to the left and add in our new values
	this->lrmem = ((this->lrmem & 0x03) << 2) + 2 * l + r;

	// Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
	this->lrsum += TRANS[this->lrmem];

	/* encoder not in the neutral (detent) state */
	if (this->lrsum % 4 != 0)
	{
		// this->move = 0;
		return 0;
	}

	/* encoder in the neutral state - clockwise rotation*/
	if (this->lrsum == 4)
	{
		this->lrsum = 0;
		if (this->invert)
		{
			this->position--;
			return -1;
		}
		this->position++;
		return 1;
	}

	/* encoder in the neutral state - anti-clockwise rotation*/
	if (this->lrsum == -4)
	{
		this->lrsum = 0;
		if (this->invert)
		{
			this->position++;
			return 1;
		}
		this->position--;
		return -1;
	}

	// An invalid rotation has been detected - ignore
	this->lrsum = 0;
	return 0;
}
