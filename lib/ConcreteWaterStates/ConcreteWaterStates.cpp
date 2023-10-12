#include "ConcreteWaterStates.h"

void WaterOff::enter(Water *water)
{
    water->setWatering(false);
    water->useLed()->On();
}

void WaterOff::execute(Water *water)
{
    // Off => On
}

void WaterOff::exit(Water *water)
{
}

WaterState &WaterOff::getInstance()
{
    static WaterOff instance;
    return instance;
}

void WaterOn::enter(Water *water)
{
    water->setWatering(true);
    water->useLed()->Blink(500, 1500).Forever();
}

void WaterOn::execute(Water *water)
{
}

void WaterOn::exit(Water *water)
{
    water->setWatering(false);
}

WaterState &WaterOn::getInstance()
{
    static WaterOn instance;
    return instance;
}