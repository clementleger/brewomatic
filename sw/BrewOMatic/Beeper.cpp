#include "Beeper.h"
#include "Arduino.h"

Beeper::Beeper()
{
	pinMode(BEEPER_PIN, OUTPUT);
}

int Beeper::beep(int note, int duration)
{
	tone(BEEPER_PIN, note, duration);
}

int Beeper::click()
{
	tone(BEEPER_PIN, NOTE_G6, BEEPER_CLICK_DURATION);
}
