#include <MotionSensor.h>

MotionSensor::MotionSensor(int pin, Stair * stair) 
{
  _stair = stair;
  _pin = pin;
  pinMode(_pin, INPUT);
}

MotionSensor::~MotionSensor()
{

}

void MotionSensor::tick() 
{
  if(millis() - lastCheck > 20) {
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
}
