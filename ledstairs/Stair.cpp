#include "Stair.h"
#define DBG Serial


uint8_t pwmMap[16] = { 0, 2, 4, 6, 8, 10, 12 ,14, 15, 13, 11, 9, 7, 5, 3, 1 };
Stair::Stair()
{

	
}


Stair::~Stair()
{
}

void Stair::lightUp(uint8_t led, int16_t intensity) {
	if (led >= 0 && led < LED_COUNT) {
		_leds[led]->lightUp(intensity);
	}
}

void Stair::lightUp(uint8_t led, int16_t intensity, long time) {
	if (led >= 0 && led < LED_COUNT) {
		_leds[led]->lightUp(intensity, time);
	}
}

void Stair::lightUpUntil(uint8_t led, int16_t intensity, long time) {
	if (led >= 0 && led < LED_COUNT) {
		_leds[led]->lightUpUntil(intensity, time);
	}
}

void Stair::lightNext() {
	lightUp(_sequenceIndex++, 4000, 100000);
	tick();
}

void Stair::flash() {
	long upto = millis() + 1000;
	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		lightUpUntil(i, 4000, upto);
	}
	tick();
}

void Stair::begin() {
	DBG.println("[Stair] begin");
	_sequenceIndex = 0;
	_functionInitTime = millis();
	_pwm.begin();
	_pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

	uint8_t twbrbackup = TWBR;
	TWBR = 12; // upgrade to 400KHz!

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		_leds[i] = new Led(pwmMap[i]);
	}

	lightNext();
}

void Stair::tick() {
	//unsigned long now = millis();

	//int16_t t = (now - _functionInitTime) % 10000 ;
	//float angle = ((float)t / 10000) * 6.28;
	//for (uint8_t i = 0; i < LED_COUNT; i++)
	//{
	//	float phase = (2.0 * 3.1415 / 16.0) * (float)i;
	//	_leds[i]->setIntensity((int16_t)((1.0 + sin(angle-phase)) * 2048));
	//}

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		_leds[i]->tick();
	}

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		_pwm.setPWM(_leds[i]->getPin(), 0, _leds[i]->getIntensity());
	}


}



//void Stair::lightFromTop(uint8_t ledPin)
//{
//	for (int8_t pwmnum = 15; pwmnum >= ledPin; pwmnum--) {
//		lightSpikeUp(pwmnum);
//		lightSpikeDown(pwmnum);
//	}
//	lightSpikeUp(ledPin);
//}
//
//void Stair::lightSpikeDown(uint8_t ledPin)
//{
//	for (int16_t i = 4096; i >= 0; i -= 64) {
//		setMappedPWM(ledPin, i);
//	}
//}
//
//void Stair::lightSpikeUp(uint8_t ledPin) {
//	for (uint16_t i = 0; i < 4096; i += 64) {
//		setMappedPWM(ledPin, i);
//	}
//}
//
//void Stair::smoothDown() {
//	for (int16_t i = 4096; i >= 0; i -= 1) {
//		for (int8_t ledPin = 15; ledPin >= 0; ledPin--) {
//			setMappedPWM(ledPin, i);
//		}
//	}
//}
//
//void Stair::lightAll(int16_t value) {
//
//	for (int8_t ledPin = 15; ledPin >= 0; ledPin--) {
//		setMappedPWM(ledPin, value);
//	}
//
//}
//
//void lightSequence1()
//{
//	for (int8_t ledPin = 0; ledPin < 16; ledPin++) {
//		lightFromTop(ledPin);
//	}
//	delay(5000);
//	smoothDown();
//
//}
