#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder():
encoder(ROTARY_ENCODER_PIN_A, ROTARY_ENCODER_PIN_B)
{
	 pinMode(ROTARY_ENCODER_SWITCH_PIN, INPUT_PULLUP);
	 oldPosition = 0;
}

int RotaryEncoder::getButtonPressed()
{
	int button = BUTTON_NONE;

	if (!digitalRead(ROTARY_ENCODER_SWITCH_PIN)) {
		button = BUTTON_OK;
	} else {
		long newPosition = encoder.read();

		if (newPosition < oldPosition)
			button = ROTARY_ENCODER_DIR_REVERSE ? BUTTON_PREV : BUTTON_NEXT;
		else if (newPosition > oldPosition)
			button = ROTARY_ENCODER_DIR_REVERSE ? BUTTON_NEXT : BUTTON_PREV;
 
		oldPosition = newPosition;
	}

	if (button)
		dbgOutput("Button %d pressed\n", button);
	return button;
}
