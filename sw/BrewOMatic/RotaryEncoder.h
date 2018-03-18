#ifndef _ROTARY_ENCODER_H
#define _ROTARY_ENCODER_H

#include "Input.h"
#include "BrewOMatic.h"
#include <Encoder.h>

class RotaryEncoder: public Input {
	public:
		RotaryEncoder();
		int getButtonPressed();
	private:
		Encoder encoder;
		long oldPosition;
};

#endif /* _ROTARY_ENCODER_H */
