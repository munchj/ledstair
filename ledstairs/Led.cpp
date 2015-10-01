#include "Led.h"



Led::Led(uint8_t pin)
{
	_pin = pin;
	_intensity = 0;
}


Led::~Led()
{
}

void Led::tick() {
	if(_endTime > 0) {
		if (millis() > _endTime) {
			_intensity = 0;
		}
	}
}

void Led::setIntensity(int16_t intensity) {
	_intensity = intensity;
}

int16_t Led::getIntensity() {
	return _intensity;
}
uint8_t Led::getPin() {
	return _pin;
}

void Led::lightUp(int16_t intensity) {
	_endTime = 0;
	_intensity = intensity;
}

void Led::lightUp(int16_t intensity, long time) {
	_endTime = millis() + time;
	_intensity = intensity;
}

void Led::lightUpUntil(int16_t intensity, long time) {
	_endTime = time;
	_intensity = intensity;
}
