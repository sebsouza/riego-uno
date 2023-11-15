#pragma once
#include "WaterState.h"
#include <jled.h>
#include <DS3231.h>
#include <OneButton.h>

class WaterState;

// Define the Water Context. This is the class that will be using the state
class Water
{
public:
    Water(JLed *led, DS3231 *rtc, OneButton *button);
    inline WaterState *getCurrentState() const { return currentState; }
    inline WaterState *getPreviousState() const { return previousState; }

    void execute();

    void setState(WaterState &newState);
    void setPreviousState(WaterState &previousState) { this->previousState = &previousState; }

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
    float getCurrentTemperature() const { return rtc->getTemperature(); }

    void setTemperatureThreshold(byte temperatureThreslhold) { this->temperatureThreslhold = temperatureThreslhold; }
    byte getTemperatureThreshold() const { return temperatureThreslhold; }

    JLed *useLed() const { return led; }
    OneButton *useButton() const { return button; }

    void buttonShortPress();
    void buttonDoublePress();
    void buttonLongPress();

    void alarm1Interrupt();
    void alarm2Interrupt();

    void setStateUpdated(bool stateUpdated) { this->stateUpdated = stateUpdated; }
    bool isStateUpdated() const { return stateUpdated; }

private:
    WaterState *currentState;
    WaterState *previousState;

    bool watering;

    byte waterStartMinute;
    byte waterStartSecond;
    byte waterLength;
    byte temperatureThreslhold;

    JLed *led;
    DS3231 *rtc;
    OneButton *button;

    bool stateUpdated;
};