#include "Beeper.h"
#include "Arduino.h"

Beeper::Beeper()
{
	pinMode(BEEPER_PIN, OUTPUT);
}

void Beeper::beep(int note, int duration)
{
	tone(BEEPER_PIN, note, duration);
}

void Beeper::click()
{
	tone(BEEPER_PIN, NOTE_G6, BEEPER_CLICK_DURATION);
}
