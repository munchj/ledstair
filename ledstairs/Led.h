#include <Arduino.h>

#ifndef LED_H
#define LED_H
class Led
{
private:
	uint8_t _pin;
	uint16_t _intensity;
	unsigned long _endTime;
public:
	Led(uint8_t pin);
	~Led();
public:
	void setIntensity(uint16_t);
	uint16_t getIntensity();
	uint8_t getPin();
	unsigned long getEndTime() { return _endTime; }

private:
	void _setIntensity(uint16_t);

public:
	void tick(unsigned long currentTime);

public:
	void lightUp(uint16_t intensity);
	void lightUp(uint16_t intensity, long time);
	void lightUpUntil(uint16_t intensity, long time);
};

#endif
