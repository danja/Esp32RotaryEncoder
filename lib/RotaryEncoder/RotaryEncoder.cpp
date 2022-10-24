#include <Arduino.h>
#include <RotaryEncoder.h>

#define BUTTON_BLANK_TIME 500

// Interrupt routine sets a flag when rotation is detected on any encoder
void IRAM_ATTR RotaryEncoder::onEncoderChange()
{
	RotaryEncoder::moved = true;
}

// Interrupt routine sets a flag when any button is pressed
void IRAM_ATTR RotaryEncoder::onButtonClick()
{
	RotaryEncoder::clicked = true;
}

volatile bool RotaryEncoder::moved = false;
volatile bool RotaryEncoder::clicked = false;

RotaryEncoder::RotaryEncoder() {}

void RotaryEncoder::init(int gpioCLK, int gpioDT, int gpioSW)
{
	this->gpioCLK = gpioCLK;
	this->gpioDT = gpioDT;
	this->gpioSW = gpioSW;

	pinMode(gpioCLK, INPUT_PULLDOWN);
	pinMode(gpioDT, INPUT_PULLDOWN);
	pinMode(gpioSW, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(gpioCLK), RotaryEncoder::onEncoderChange, CHANGE);
	attachInterrupt(digitalPinToInterrupt(gpioDT), RotaryEncoder::onEncoderChange, CHANGE);
	attachInterrupt(digitalPinToInterrupt(gpioSW), RotaryEncoder::onButtonClick, FALLING);
}

void RotaryEncoder::setScale(float startValue, float minValue, float maxValue, float stepSize, bool invert, bool circleValues)
{
	this->setValue(startValue);
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->stepSize = stepSize;
	this->invert = invert;
	this->circleValues = circleValues;
	this->nSteps = (maxValue - minValue) / stepSize;
}

void RotaryEncoder::setValue(float value)
{
	position = (value - minValue) / stepSize;
}

// I don't like the side effects!
float RotaryEncoder::value()
{
	if (circleValues)
	{
		if (position > nSteps)
		{
			position = 0;
		}
		if (position < 0)
		{
			position = nSteps;
		}
	}
	else
	{
		if (position > nSteps)
		{
			position = nSteps;
		}
		if (position < 0)
		{
			position = 0;
		}
	}

	float value = ((float)position) * stepSize + minValue;

	return value;
}

bool RotaryEncoder::readButton()
{
	long elapsed = millis() - this->timer;

	if (elapsed < BUTTON_BLANK_TIME)
	{
		RotaryEncoder::clicked = false;
		return false;
	}
	if (!RotaryEncoder::clicked)
	{
		return false;
	}

	int8_t button = digitalRead(this->gpioSW);

	if (!button)
	{
		return false;
	}

	this->timer = millis();
	RotaryEncoder::clicked = false;

	return true;
}

// Rotary encoder has moved (interrupt tells us) but what happened?
// See https://www.pinteric.com/rotary.html
int RotaryEncoder::readEncoder()
{
	// Reset the flag that brought us here (from ISR)
	RotaryEncoder::moved = false;

	static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

	// Read BOTH pin states to determine validity of rotation
	// portENTER_CRITICAL_ISR(&(this->encoderMux));
	int8_t l = digitalRead(this->gpioCLK);
	int8_t r = digitalRead(this->gpioDT);
	// portEXIT_CRITICAL_ISR(&(this->encoderMux));

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
