#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver\Adafruit_PWMServoDriver.h>
#include "Led.h"
#define LED_COUNT 16

#ifndef STAIR
#define STAIR

enum StairMode {
	DEFAULT_LIGHT_MODE,
	LIGHT_MAX,
	LIGHT_SEQUENCE_1,
	LIGHT_SEQUENCE_2,
	SINUSOIDE,
	SINUSOIDE2,
	STATIC,
	VUMETER,
	VUMETER2,
	LIGHT_SEQUENCE_3,
	WIFI_INIT
	
};

class EspWifi;

struct LightSequence2Helper {
	long lastTick;
	uint8_t index1;
	uint8_t index2;
};

class Stair
{
public:
	Stair();
	~Stair();

public:
	void begin();
	void tick();
	void nextMode();
	void setMode(int mode);
	void nextIntensity();
	void setIntensity(int intensity);
	void setWifi(EspWifi * wifi) { _wifi = wifi; }

private:
	EspWifi * _wifi;
	unsigned long _functionInitTime;
	Led* _leds[LED_COUNT];
	Adafruit_PWMServoDriver _pwm;
	StairMode _mode;
	struct LightSequence2Helper _lightSequence2Helper;
	uint8_t _sensorTriggered;
	int _maxIntensity;
	
public:
	void setMode(StairMode mode) {_mode = mode;}
	StairMode getMode() {return _mode;}
	void setMaxIntensity(int intensity) { _maxIntensity = intensity; }

public:
	void lightUp(uint8_t led, int16_t intensity);
	void lightUp(uint8_t led, int16_t intensity, long time);
	void lightUpUntil(uint8_t led, int16_t intensity, long time);
	void lightNext();
	void flash();
	
public:
	void motionDetected(uint8_t id);
};


#endif
