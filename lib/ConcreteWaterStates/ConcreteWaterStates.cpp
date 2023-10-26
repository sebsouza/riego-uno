#include "ConcreteWaterStates.h"

void WaterOff::enter(Water *water)
{
    water->setWatering(false);
    water->useLed()->On();
}

void WaterOff::execute(Water *water)
{
}

void WaterOff::exit(Water *water)
{
}

void WaterOff::buttonShortPress(Water *water)
{
    water->setState(WaterOn::getInstance());
}

void WaterOff::buttonLongPress(Water *water)
{
    water->setState(WaterConfig::getInstance());
}

WaterState &WaterOff::getInstance()
{
    static WaterOff instance;
    return instance;
}

void WaterOn::enter(Water *water)
{
    water->setWatering(true);
    water->setWaterStartMinute(water->getCurrentMinute());
    water->useLed()->Blink(1500, 500).Forever();
}

void WaterOn::execute(Water *water)
{
}

void WaterOn::exit(Water *water)
{
    water->setWatering(false);
}

void WaterOn::buttonShortPress(Water *water)
{
    water->setState(WaterOff::getInstance());
}

void WaterOn::buttonLongPress(Water *water)
{
    water->setState(WaterConfig::getInstance());
}

WaterState &WaterOn::getInstance()
{
    static WaterOn instance;
    return instance;
}

void WaterConfig::enter(Water *water)
{
    byte currentWaterLength = water->getWaterLength();
    byte lengthUnit = currentWaterLength % 10;
    byte lengthTens = currentWaterLength / 10;

    water->useLed()->Blink(750, 250).Repeat(lengthTens).Blink(250, 250).Repeat(lengthUnit).DelayAfter(2500).Forever();
}

void WaterConfig::execute(Water *water)
{
}

void WaterConfig::exit(Water *water)
{
    water->useLed()->On();
}

void WaterConfig::buttonShortPress(Water *water)
{
    water->setState(WaterOff::getInstance());
}

void WaterConfig::buttonLongPress(Water *water)
{
    water->setState(WaterOn::getInstance());
}

WaterState &WaterConfig::getInstance()
{
    static WaterConfig instance;
    return instance;
}