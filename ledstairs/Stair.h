#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include "Led.h"
#define LED_COUNT 16

#ifndef STAIR
#define STAIR

enum StairMode {
	DEFAULT,
	LIGHT_SEQUENCE_1,
	LIGHT_SEQUENCE_2,
	SINUSOIDE,
	STATIC
};

struct LightSequence2Helper {
	long lastTick;
	uint8_t index1;
	uint8_t index2;
}

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
	StairMode _mode;
	struct LightSequence2Helper _lightSequence2Helper;
	
public:
	void setMode(StairMode mode) {_mode = mode;}
	StairMode getMode() {return _mode;}

public:
	void lightUp(uint8_t led, int16_t intensity);
	void lightUp(uint8_t led, int16_t intensity, long time);
	void lightUpUntil(uint8_t led, int16_t intensity, long time);
	void lightNext();
	void flash();
};


#endif
