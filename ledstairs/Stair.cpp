#include "Stair.h"
#include <Constants.h>



char * StairModeName[] =
{
	"DEFAULT_LIGHT_MODE",
	"LIGHT_MAX",
	"LIGHT_SEQUENCE_1",
	"LIGHT_SEQUENCE_2",
	"SINUSOIDE",
	"STATIC",
	"VUMETER",
	"VUMETER2"
};

//uint8_t pwmMap[16] = { 0, 2, 4, 6, 8, 10, 12 ,14, 1, 3, 5, 7, 9, 11, 13, 15 };
//uint8_t pwmMap[16] = { 0, 2, 4, 6, 8, 10, 12 ,14, 15, 13, 11, 9, 7, 5, 3, 1 };
//uint8_t pwmMap[16] = { 1,3,5,7,9,11,13,15,14,12,10,8,6,4,2,0 };
uint8_t pwmMap[16] = {2,4,6,8,10,12,14,15,13,11,9,7,5,3,1, 0 };

Stair::Stair()
{
	_mode = SINUSOIDE;
	_maxIntensity = 128;
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

void Stair::flash() {
	long upto = millis() + 50;
	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		lightUpUntil(i, 4000, upto);
	}
	_functionInitTime = millis();
}

void Stair::nextMode()
{
	switch(_mode) {
	case SINUSOIDE:
		_mode = LIGHT_SEQUENCE_1;
		break;
	case LIGHT_SEQUENCE_1:
		_mode = LIGHT_SEQUENCE_2;
		break;
	case LIGHT_SEQUENCE_2:
		_mode = VUMETER;
		break;
	case VUMETER:
		_mode = VUMETER2;
		break;
	case VUMETER2:
		_mode = LIGHT_MAX;
		break;
	case LIGHT_MAX:
		_mode = DEFAULT_LIGHT_MODE;
		break;
	case DEFAULT_LIGHT_MODE:
		_mode = SINUSOIDE;
		break;
	}

	DBG.print("[Stair][nextMode]");
	DBG.println(StairModeName[_mode]);
	_functionInitTime = millis();
	_lightSequence2Helper = LightSequence2Helper();
}

void Stair::nextIntensity() {
	_maxIntensity *= 2;
	if (_maxIntensity == 4096) {
		_maxIntensity = 4095;
	}
	else if (_maxIntensity > 4096) {
		_maxIntensity = 128;
	}
	DBG.print("[Stair][nextMode]");
	DBG.println(_maxIntensity);
}

void Stair::begin() {
	DBG.println("[Stair] begin");
	_functionInitTime = millis();
	DBG.println("[Stair] pwm begin");
	_pwm.begin();
	DBG.println("[Stair] set freq");
	_pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

	uint8_t twbrbackup = TWBR;
	TWBR = 12; // upgrade to 400KHz!

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		_leds[i] = new Led(pwmMap[i]);
	}

	DBG.println("[Stair] begin ok");
}

void Stair::motionDetected(uint8_t id) {
	long now = millis();
	DBG.print("[Stair] motion detected :");
	DBG.println(id);
	if (now - _functionInitTime > 20000) {
		DBG.println("[Stair] trigger");
		_sensorTriggered = id;
		_functionInitTime = now;
	}
}

void Stair::tick() {

	unsigned long now = millis();

	switch(_mode) {
	case DEFAULT_LIGHT_MODE:
	{
		for (uint8_t i = 0; i < LED_COUNT; i++)
		{
			_leds[i]->setIntensity(0);
		}
		break;
	}
	case LIGHT_MAX:
	{
		for (uint8_t i = 0; i < LED_COUNT; i++)
		{
			_leds[i]->setIntensity(4095);
		}
		break;
	}
		case SINUSOIDE:
		{
			int16_t t = (now - _functionInitTime) % 2000 ;
			float angle = ((float)t / 2000.0) * 6.28;
			for (uint8_t i = 0; i < LED_COUNT; i++)
			{
				float phase = (2.0 * 3.1415 / (float)LED_COUNT) * (float)i;
				_leds[i]->setIntensity((int16_t)((1.0 + sin(angle-phase)) * _maxIntensity/2.0));
			}
			break;
		}
		case VUMETER:
		{
			
			int val = analogRead(A13);
			
			int computed = (val-253) * 15 /12 ;
			if (computed < 0) {
				computed = 0;
			}
			if (computed > 15) {
				computed = 15;
			}

			//for (int i = 0; i <= computed; i++) {
			//	DBG.print('.');
			//}
			//DBG.println("");

			for (uint8_t i = 0; i < LED_COUNT; i++)
			{
				if (computed >= i) {
					_leds[LED_COUNT - i]->setIntensity (_maxIntensity);
				}
				else {
					_leds[LED_COUNT - i]->setIntensity(0);
				}
			}
			break;
		}
		case VUMETER2:
		{

			int val = analogRead(A13);

			int computed = (val - 253) * 15 / 12;
			if (computed < 0) {
				computed = 0;
			}
			if (computed > 15) {
				computed = 15;
			}

			//for (int i = 0; i <= computed; i++) {
			//	DBG.print('.');
			//}
			//DBG.println("");

			long t = now + 50;
			for (uint8_t i = 0; i < LED_COUNT; i++)
			{
				
				if (computed >= i) {
					_leds[LED_COUNT - i]->lightUpUntil(_maxIntensity, t);
				}
			}
			break;
		}
		case LIGHT_SEQUENCE_1:
		{
			int16_t lightUpDuration = 900;
			int16_t lightDownDuration = 3000;
			int16_t totalUpDuration = 15000;
			int16_t phaseDuration = 200;
			long t = (now - _functionInitTime);
			for (uint8_t i = 0; i < LED_COUNT; i++)
			{
				long tChannel = t - phaseDuration * i; //t + phase
				uint8_t ledIndex = _sensorTriggered == A1 ? i : 15 - i;
				
				if(tChannel < 0 ||  tChannel > (lightUpDuration+lightDownDuration+totalUpDuration))
				{
					//DBG.print(tChannel); DBG.println("...1");
					_leds[ledIndex]->setIntensity(0);
				}
				else if(tChannel > 0 && tChannel <= lightUpDuration)
				{
					//DBG.print(tChannel); DBG.println("...2");
					_leds[ledIndex]->setIntensity(tChannel  * _maxIntensity / lightUpDuration);
				}
				else if(tChannel > lightUpDuration && tChannel <= (lightUpDuration+totalUpDuration))
				{
					//DBG.print(tChannel); DBG.println("...3");
					_leds[ledIndex]->setIntensity(_maxIntensity);
				}
				else if(tChannel > (lightUpDuration+totalUpDuration) && tChannel <= (lightUpDuration+totalUpDuration+lightDownDuration))
				{
					//DBG.print(tChannel); DBG.println("...4");
					_leds[ledIndex]->setIntensity((lightUpDuration+totalUpDuration+lightDownDuration-tChannel)  * _maxIntensity / lightDownDuration);
				}
				
			}
			break;
		}	
		case LIGHT_SEQUENCE_2:
		{
			if(now - _lightSequence2Helper.lastTick > 30)
			{
				_lightSequence2Helper.lastTick = now;
				for (uint8_t i = 0; i < LED_COUNT; i++)
				{
					uint8_t ledIndex = _sensorTriggered == A0 ? i : 15 - i;
					if(_lightSequence2Helper.index1 > i || 15-i==_lightSequence2Helper.index2)
					{
						_leds[ledIndex]->setIntensity(_maxIntensity);
					}
					else
					{
						_leds[ledIndex]->setIntensity(0);
					}
					
				}
				
				_lightSequence2Helper.index2++;
				if(_lightSequence2Helper.index2 == 16 - _lightSequence2Helper.index1)
				{
					_lightSequence2Helper.index2 = 0;
					_lightSequence2Helper.index1++;
				}
			}
			break;
		}		
		default:
		{
			
		}
	}

//	now = millis();

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		_leds[i]->tick(now);
	}

	for (uint8_t i = 0; i < LED_COUNT; i++)
	{
		delayMicroseconds(2000);
		//DBG.print("<");
		//DBG.print(_leds[i]->getIntensity());
		_pwm.setPWM(_leds[i]->getPin(), 0, _leds[i]->getIntensity());
		//DBG.println(">");
	}


}
