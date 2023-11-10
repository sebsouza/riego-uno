#pragma once
#include "Water.h"

class Water;

// Define the Water Interface. This is the class that defines the interface for the Water States
class WaterState
{
public:
    virtual void enter(Water *water) = 0;
    virtual void execute(Water *water) = 0;
    virtual void exit(Water *water) = 0;

    virtual void buttonShortPress(Water *water) = 0;
    virtual void buttonDoublePress(Water *water) = 0;
    virtual void buttonLongPress(Water *water) = 0;

    virtual ~WaterState() {}
};