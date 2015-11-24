#include "Led.h"



Led::Led(uint8_t pin)
{
	_pin = pin;
	_intensity = 0;
}


Led::~Led()
{
}

void Led::tick(unsigned long currentTime) {
	if(_endTime > 0) {
		Serial.print(_pin);
		Serial.print("/");
		Serial.print(currentTime);
		Serial.print(" ");
		Serial.println(_endTime);

		if (currentTime > _endTime) {

			_intensity = 0;
		}
	}
}

void Led::setIntensity(int16_t intensity) {
	_endTime = 0;
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
