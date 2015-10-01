#include <Arduino.h>

#ifndef LED_H
#define LED_H
class Led
{
private:
	uint8_t _pin;
	int16_t _intensity;
	long _endTime;
public:
	Led(uint8_t pin);
	~Led();
public:
	void setIntensity(int16_t);
	int16_t getIntensity();
	uint8_t getPin();
	long getEndTime() { return _endTime; }

public:
	void tick();

public:
	void lightUp(int16_t intensity);
	void lightUp(int16_t intensity, long time);
	void lightUpUntil(int16_t intensity, long time);
};

#endif
