
#include <Esp32RotaryEncoder.h>

Esp32RotaryEncoder::Esp32RotaryEncoder(uint8_t gpioCLK, uint8_t gpioDT, uint8_t gpioSW)
{
	this->old_AB = 0;
	this->lastReadEncoder0Pos = 0;

	this->gpioCLK = gpioCLK;
	this->gpioDT = gpioDT;
	this->gpioSW = gpioSW;
	this->encoderSteps = 10;

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

	// this->encoder0Pos += ( this->enc_states[( this->old_AB & 0x0f )]);
	int8_t currentDirection = (this->enc_states[(this->old_AB & 0x0f)]); //-1,0 or 1

	if (currentDirection != 0)
	{
		long prevRotaryPosition = this->encoder0Pos / this->encoderSteps;
		this->encoder0Pos += currentDirection;
		long newRotaryPosition = this->encoder0Pos / this->encoderSteps;

		if (newRotaryPosition != prevRotaryPosition && rotaryAccelerationCoef > 1)
		{
			// additional movements cause acceleration?
			//  at X ms, there should be no acceleration.
			unsigned long accelerationLongCutoffMillis = 200;
			// at Y ms, we want to have maximum acceleration
			unsigned long accelerationShortCutffMillis = 4;

			// compute linear acceleration
			if (currentDirection == lastMovementDirection &&
				currentDirection != 0 &&
				lastMovementDirection != 0)
			{
				// ... but only of the direction of rotation matched and there
				// actually was a previous rotation.
				unsigned long millisAfterLastMotion = now - lastMovementAt;

				if (millisAfterLastMotion < accelerationLongCutoffMillis)
				{
					if (millisAfterLastMotion < accelerationShortCutffMillis)
					{
						millisAfterLastMotion = accelerationShortCutffMillis; // limit to maximum acceleration
					}
					if (currentDirection > 0)
					{
						this->encoder0Pos += rotaryAccelerationCoef / millisAfterLastMotion;
					}
					else
					{
						this->encoder0Pos -= rotaryAccelerationCoef / millisAfterLastMotion;
					}
				}
			}
			this->lastMovementAt = now;
			this->lastMovementDirection = currentDirection;
		}

		// respect limits
		if (this->encoder0Pos > (this->_maxEncoderValue))
			this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
		if (this->encoder0Pos < (this->_minEncoderValue))
			this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;
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

void Esp32RotaryEncoder::setBoundaries(long minEncoderValue, long maxEncoderValue, bool circleValues)
{
	this->_minEncoderValue = minEncoderValue * this->encoderSteps;
	this->_maxEncoderValue = maxEncoderValue * this->encoderSteps;

	this->_circleValues = circleValues;
}

long Esp32RotaryEncoder::getValue()
// long Esp32RotaryEncoder::readEncoder()
{
	return (this->encoder0Pos / this->encoderSteps);
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
	newValue_ = newValue_ * this->encoderSteps;
	this->encoder0Pos = newValue_;
	this->lastReadEncoder0Pos = this->encoder0Pos;
	if (this->encoder0Pos > this->_maxEncoderValue)
		this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
	if (this->encoder0Pos < this->_minEncoderValue)
		this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;
}
