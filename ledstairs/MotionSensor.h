#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <Arduino.h>
#include <Stair.h>

class MotionSensor
{
  public:
    MotionSensor(int pin, Stair * stair);
    ~MotionSensor();
    
  private:
    Stair * _stair;
    int _pin;
    bool _activated;
    long _lastCheck;
    
  public:
    void tick();
};

#endif
