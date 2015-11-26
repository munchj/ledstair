#include "Led.h"

#define DBG Serial

Led::Led(uint8_t pin)
{
	_pin = pin;
	_intensity = 0;
	_endTime = 0;
}


Led::~Led()
{
}

void Led::tick(unsigned long currentTime) {
	//Serial.print(_pin);
	//Serial.print("/");
	//Serial.print(currentTime);
	//Serial.print(" ");
	//Serial.println(_endTime);
	if(_endTime > 0) {
		if (currentTime > _endTime) {
			setIntensity(0);
		}
	}
}

void Led::_setIntensity(int16_t intensity) {
	_intensity = intensity;

	if (_intensity > 4095) {
		_intensity = 4095;
	}
	else if (_intensity < 0) {
		_intensity = 0;
	}
}

void Led::setIntensity(int16_t intensity) {
	//DBG.print("[led][");
	//DBG.print(_pin);
	//DBG.print("][setIntensity] ");
	//DBG.println(intensity);
	_endTime = 0;
	_setIntensity(intensity);
}

int16_t Led::getIntensity() {
	//DBG.print("[led][");
	//DBG.print(_pin);
	//DBG.print("][getIntensity] ");
	//DBG.println(_intensity);
	return _intensity;
}
uint8_t Led::getPin() {
	return _pin;
}

void Led::lightUp(int16_t intensity) {
	_endTime = 0;
	_setIntensity(intensity);
}

void Led::lightUp(int16_t intensity, long time) {
	_endTime = millis() + time;
	_setIntensity(intensity);
}

void Led::lightUpUntil(int16_t intensity, long time) {
	_endTime = time;
	_setIntensity(intensity);
}
