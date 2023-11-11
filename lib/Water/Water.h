#pragma once
#include "WaterState.h"
#include <jled.h>
#include <DS3231.h>

class WaterState;

// Define the Water Context. This is the class that will be using the state
class Water
{
public:
    Water(JLed *led, DS3231 *rtc);
    inline WaterState *getCurrentState() const { return currentState; }
    inline WaterState *getPreviousState() const { return previousState; }
    void execute();

    void setState(WaterState &newState);

    void setWatering(bool watering) { this->watering = watering; }
    bool isWatering() const { return watering; }

    void setWaterStartMinute(byte waterStartMinute) { this->waterStartMinute = waterStartMinute; }
    byte getWaterStartMinute() const { return waterStartMinute; }

    void setWaterStartSecond(byte waterStartSecond) { this->waterStartSecond = waterStartSecond; }
    byte getWaterStartSecond() const { return waterStartSecond; }

    void setWaterLength(byte waterLength) { this->waterLength = waterLength; }
    byte getWaterLength() const { return waterLength; }

    byte getCurrentMinute() const { return rtc->getMinute(); }
    byte getCurrentSecond() const { return rtc->getSecond(); }

    JLed *useLed() const { return led; }

    void buttonShortPress();
    void buttonDoublePress();
    void buttonLongPress();

private:
    WaterState *currentState;
    WaterState *previousState;
    bool watering;
    byte waterStartMinute;
    byte waterStartSecond;
    byte waterLength; // TODO: Implement this
    JLed *led;
    DS3231 *rtc;
};