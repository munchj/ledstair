#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include "Led.h"
#define LED_COUNT 16

#ifndef STAIR
#define STAIR

class Stair
{
public:
	Stair();
	~Stair();

public:
	void begin();
	void tick();
	

private:
	unsigned long _functionInitTime;
	Led* _leds[LED_COUNT];
	Adafruit_PWMServoDriver _pwm;
	uint8_t _sequenceIndex;

public:
	void lightUp(uint8_t led, int16_t intensity, long time);
	void lightUpUntil(uint8_t led, int16_t intensity, long time);
	void lightNext();
	void flash();

//private:
//	void lightFromTop(uint8_t ledPin);
//	void lightSpikeDown(uint8_t ledPin);
//	void lightSpikeUp(uint8_t ledPin);
//	void smoothDown();
//	void lightSequence1();
};


#endif