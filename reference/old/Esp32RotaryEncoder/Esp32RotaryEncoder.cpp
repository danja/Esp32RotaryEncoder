
#include <Esp32RotaryEncoder.h>

Esp32RotaryEncoder::Esp32RotaryEncoder(uint8_t gpioCLK, uint8_t gpioDT, uint8_t gpioSW)
{
	this->old_AB = 0;
	this->lastReadEncoder0Pos = 0;

	this->gpioCLK = gpioCLK;
	this->gpioDT = gpioDT;
	this->gpioSW = gpioSW;
	this->steps = 10;

	pinMode(this->gpioCLK, INPUT_PULLDOWN);
	pinMode(this->gpioDT, INPUT_PULLDOWN);
	pinMode(this->gpioSW, INPUT_PULLDOWN);
}

void Esp32RotaryEncoder::setup(void (*ISR_encoder)(void), void (*ISR_button)(void))
{
	attachInterrupt(digitalPinToInterrupt(this->gpioCLK), ISR_encoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->gpioDT), ISR_encoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->gpioSW), ISR_button, RISING);
}

void IRAM_ATTR Esp32RotaryEncoder::updateValue()
{
	unsigned long now = millis();

	portENTER_CRITICAL_ISR(&(this->mux));

	this->old_AB <<= 2; // remember previous state

	int8_t ENC_PORT = ((digitalRead(this->gpioDT)) ? (1 << 1) : 0) | ((digitalRead(this->gpioCLK)) ? (1 << 0) : 0);

	this->old_AB |= (ENC_PORT & 0x03); // add current state

	int8_t currentDirection = (this->enc_states[(this->old_AB & 0x0f)]); //-1,0 or 1

	if (currentDirection != 0)
	{
		long prevRotaryPosition = this->encoder0Pos / this->steps;
		this->encoder0Pos += currentDirection;
		long newRotaryPosition = this->encoder0Pos / this->steps;

		// respect limits
		if (this->encoder0Pos > (this->maxValue))
			this->encoder0Pos = this->circleValues ? this->minValue : this->maxValue;
		if (this->encoder0Pos < (this->minValue))
			this->encoder0Pos = this->circleValues ? this->maxValue : this->minValue;
	}
	this->valueChangeFlag = true;
	portEXIT_CRITICAL_ISR(&(this->mux));
}

boolean Esp32RotaryEncoder::valueChanged()
{
	return this->valueChangeFlag;
}
void Esp32RotaryEncoder::resetValue()
{
	this->valueChangeFlag = false;
}

void IRAM_ATTR Esp32RotaryEncoder::readButton_ISR()
{
	portENTER_CRITICAL_ISR(&(this->buttonMux));
	this->buttonClickedFlag = true;
	portEXIT_CRITICAL_ISR(&(this->buttonMux));
}

boolean Esp32RotaryEncoder::buttonClicked()
{
	return this->buttonClickedFlag;
}

void Esp32RotaryEncoder::resetButton()
{
	this->buttonClickedFlag = false;
}

/*
void Esp32RotaryEncoder::setBoundaries(long minEncoderValue, long maxEncoderValue, bool circleValues)
{
	this->minValue = minEncoderValue * this->steps;
	this->maxValue = maxEncoderValue * this->steps;

	this->circleValues = circleValues;
}
*/

void Esp32RotaryEncoder::setScale(long minValue, long maxValue, long steps, bool invert, bool circleValues)
{
	this->steps = steps;
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->invert = invert;
	this->circleValues = circleValues;
}

long Esp32RotaryEncoder::getValue()
{
	Serial.println(" ");
	Serial.print("this->minValue = ");
	Serial.println(this->minValue);
	Serial.print("this->maxValue = ");
	Serial.println(this->maxValue);

	Serial.print("encoder0Pos = ");
	Serial.println(encoder0Pos);

	// long value = (this->encoder0Pos / this->steps);
	long value = this->encoder0Pos;
	Serial.print("value = ");
	Serial.println(value);

	if (this->invert)
	{
		return this->maxValue - value;
	}
	return value;
}

void Esp32RotaryEncoder::setEncoderValue(long newValue)
{
	reset(newValue);
}

/*
long Esp32RotaryEncoder::encoderChanged()
{
	long _encoder0Pos = readEncoder();
	long encoder0Diff = _encoder0Pos - this->lastReadEncoder0Pos;

	this->lastReadEncoder0Pos = _encoder0Pos;

	return encoder0Diff;
}
*/

void Esp32RotaryEncoder::reset(long newValue_)
{
	newValue_ = newValue_ * this->steps;
	this->encoder0Pos = newValue_;
	this->lastReadEncoder0Pos = this->encoder0Pos;
	if (this->encoder0Pos > this->maxValue)
		this->encoder0Pos = this->circleValues ? this->minValue : this->maxValue;
	if (this->encoder0Pos < this->minValue)
		this->encoder0Pos = this->circleValues ? this->maxValue : this->minValue;
}
