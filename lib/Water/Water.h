#pragma once
#include "WaterState.h"
#include <jled.h>
#include <DS3231.h>
#include <OneButton.h>
#include "Buzzer.h"

class WaterState;

// Define the Water Context. This is the class that will be using the state
class Water
{
public:
    Water(JLed *led, DS3231 *rtc, OneButton *button, Buzzer *buzzer);
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

    void setAlarmTime(byte alarmTime) { this->alarmTime = alarmTime; }
    byte getAlarmTime() const { return alarmTime; }

    JLed *useLed() const { return led; }
    OneButton *useButton() const { return button; }
    Buzzer *useBuzzer() const { return buzzer; }
    DS3231 *useRtc() const { return rtc; }

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
    byte alarmTime; // 0: 6, 1: 12, 2: 18, 3: 0

    JLed *led;
    DS3231 *rtc;
    OneButton *button;
    Buzzer *buzzer;

    bool stateUpdated;
};