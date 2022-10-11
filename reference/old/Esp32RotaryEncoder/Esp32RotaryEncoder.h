#ifndef _ESP32ROTARYENCODER_h
#define _ESP32ROTARYENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Esp32RotaryEncoder
{

private:
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portMUX_TYPE buttonMux = portMUX_INITIALIZER_UNLOCKED;

	volatile long encoder0Pos = 0;

	volatile int8_t lastMovementDirection = 0; // 1 right; -1 left
	volatile unsigned long lastMovementAt = 0;
	unsigned long rotaryAccelerationCoef = 150;

	bool circleValues = false;

	uint8_t gpioCLK = 34;
	uint8_t gpioDT = 35;
	uint8_t gpioSW = 36;

	long steps = 10;
	bool invert = false;

	long minValue = -1 << 15;
	long maxValue = 1 << 15;

	uint8_t old_AB;
	long lastReadEncoder0Pos;

	int8_t enc_states[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
	void (*ISR_callback)();
	void (*ISR_button)();

public:
	// Esp32RotaryEncoder();
	Esp32RotaryEncoder(uint8_t gpioCLK, uint8_t gpioDT, uint8_t gpioSW);

	// void setBoundaries(long minValue = -100, long maxValue = 100, bool circleValues = false);
	void setScale(long minValue = 0, long maxValue = 100, long steps = 100, boolean invert = true, bool circleValues = true);

	void IRAM_ATTR updateValue();
	void IRAM_ATTR readButton_ISR();

	volatile boolean valueChangeFlag = false; // not 100% about volatile
	volatile boolean buttonClickedFlag = false;

	boolean valueChanged();
	boolean buttonClicked();

	void resetValue();
	void resetButton();

	void setup(void (*ISR_callback)(void), void (*ISR_button)(void));
	void reset(long newValue = 0);

	long getValue();
	void setEncoderValue(long newValue);

	long encoderChanged();
};
#endif
