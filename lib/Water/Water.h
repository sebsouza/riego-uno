#pragma once
#include "WaterState.h"
#include <jled.h>

class WaterState;

enum LedStates
{
    OFF,
    ON,
    BLINK
};

// Define the Water Context. This is the class that will be using the state
class Water
{
public:
    Water(JLed *led);
    inline WaterState *getCurrentState() const { return currentState; }
    void execute();
    void setState(WaterState &newState);
    void setWatering(bool watering) { this->watering = watering; }
    bool isWatering() const { return watering; }
    JLed *useLed() const { return led; }

private:
    WaterState *currentState;
    bool watering;
    byte waterLength; // TODO: Implement this
    JLed *led;
};