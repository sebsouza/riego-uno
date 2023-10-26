#pragma once
#include "WaterState.h"
#include <jled.h>
#include <DS3231.h>

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
    Water(JLed *led, DS3231 *rtc, byte waterLength);
    inline WaterState *getCurrentState() const { return currentState; }
    void execute();

    void setState(WaterState &newState);

    void setWatering(bool watering) { this->watering = watering; }
    bool isWatering() const { return watering; }

    void setWaterStartMinute(byte waterStartMinute) { this->waterStartMinute = waterStartMinute; }
    byte getWaterStartMinute() const { return waterStartMinute; }

    void setWaterLength(byte waterLength) { this->waterLength = waterLength; }
    byte getWaterLength() const { return waterLength; }

    byte getCurrentMinute() const { return rtc->getMinute(); }

    JLed *useLed() const { return led; }

    void buttonShortPress();
    void buttonLongPress();
    void buttonDoublePress();
    void buttonTriplePress();

private:
    WaterState *currentState;
    bool watering;
    byte waterStartMinute;
    byte waterLength; // TODO: Implement this
    JLed *led;
    DS3231 *rtc;
};