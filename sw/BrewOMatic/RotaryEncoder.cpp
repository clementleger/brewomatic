#include "RotaryEncoder.h"

int RotaryEncoder::setup()
{
	 encoder = new Encoder(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B);
	 pinMode(ROTARY_ENCODER_SWITCH_PIN, INPUT);
	 oldPosition = 0;

	 return 0;
}

int RotaryEncoder::getButtonPressed()
{
	if (digitalRead(ROTARY_ENCODER_SWITCH_PIN))
		return BUTTON_OK;

	long newPosition = encoder->read();

	if (newPosition < oldPosition)
		return ROTARY_ENCODER_DIR_REVERSE ? BUTTON_PREV : BUTTON_NEXT;
	else if (newPosition > oldPosition)
		return ROTARY_ENCODER_DIR_REVERSE ? BUTTON_NEXT : BUTTON_PREV;

	oldPosition = newPosition;

	return BUTTON_NONE;
}
