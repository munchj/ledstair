#include <MotionSensor.h>
#include <Constants.h>

MotionSensor::MotionSensor(int pin) 
{
  _pin = pin;
  pinMode(_pin, INPUT);
}

MotionSensor::~MotionSensor()
{

}

void MotionSensor::tick() 
{
	  if(digitalRead(_pin) == HIGH)
      {
        if(!_activated)
        {
          _activated = true;
          _stair->motionDetected(_pin);
        }
      }
      else 
      {
        if(_activated)
        {
          _activated = false;
        }
      }
}
