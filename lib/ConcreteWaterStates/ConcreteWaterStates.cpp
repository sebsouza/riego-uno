#include <EEPROM.h>
#include "ConcreteWaterStates.h"
#include "services.cpp"

// Idle implementation

void Idle::enter(Water *water)
{
    water->setWatering(false);
    water->useLed()->On();
    Serial.println("Idle::enter");
}

void Idle::execute(Water *water)
{
}

void Idle::exit(Water *water)
{
}

void Idle::buttonShortPress(Water *water)
{
    water->setState(Watering::getInstance());
    Serial.println("Button short press");
}

void Idle::buttonDoublePress(Water *water)
{
    water->setState(RainDetected::getInstance());
}

void Idle::buttonLongPress(Water *water)
{
    water->setState(Settings::getInstance());
}

void Idle::alarm1Interrupt(Water *water)
{
    water->setState(Watering::getInstance());
}

WaterState &Idle::getInstance()
{
    static Idle instance;
    return instance;
}

// Watering implementation

void Watering::enter(Water *water)
{
    float currentTemperature = water->getCurrentTemperature();
    Serial.print("Watering::enter. Current temperature: ");
    Serial.println(currentTemperature);

    if (!water->isWatering())
    {
        water->setWatering(true);

        byte currentMinute = water->getCurrentMinute();
        byte currentSecond = water->getCurrentSecond();
        water->setWaterStartMinute(currentMinute);
        water->setWaterStartSecond(currentSecond);
    }
}

void Watering::execute(Water *water)
{
    blinkRemainingTime(water);

    checkWateringTime(water);
}

void Watering::exit(Water *water)
{
}

void Watering::buttonShortPress(Water *water)
{
    water->setState(Idle::getInstance());
}

void Watering::buttonDoublePress(Water *water)
{
}

void Watering::buttonLongPress(Water *water)
{
    water->setState(Settings::getInstance());
}

void Watering::alarm1Interrupt(Water *water)
{
}

WaterState &Watering::getInstance()
{
    static Watering instance;
    return instance;
}

// Settings implementation

void Settings::enter(Water *water)
{
    water->useLed()->Off().Update();
    Serial.println("Settings::enter");
}

void Settings::execute(Water *water)
{
    blinkWaterLength(water);

    if (water->getPreviousState() == &Watering::getInstance())
    {
        checkWateringTime(water);
    }
}

void Settings::exit(Water *water)
{
    EEPROM.put(128, 'S');
    EEPROM.put(129, water->getWaterLength());
}

void Settings::buttonShortPress(Water *water)
{
    water->setWaterLength(water->getWaterLength() + 1);
}

void Settings::buttonDoublePress(Water *water)
{
    water->setWaterLength(water->getWaterLength() - 1);
}

void Settings::buttonLongPress(Water *water)
{
    WaterState *previousState = water->getPreviousState();
    water->setState(*previousState);
}

void Settings::alarm1Interrupt(Water *water)
{
    WaterState *previousState = water->getPreviousState();
    if (previousState == &Idle::getInstance())
    {
        water->setState(Watering::getInstance());
    }
    else if (previousState == &RainDetected::getInstance())
    {
    }
}

WaterState &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

// RainDetected implementation

void RainDetected::enter(Water *water)
{
    Serial.println("RainDetected::enter");
}

void RainDetected::execute(Water *water)
{
    water->useLed()->Blink(100, 3000).Update();
}

void RainDetected::exit(Water *water)
{
}

void RainDetected::buttonShortPress(Water *water)
{
    water->setState(Idle::getInstance());
}

void RainDetected::buttonDoublePress(Water *water)
{
}

void RainDetected::buttonLongPress(Water *water)
{
    water->setState(Settings::getInstance());
}

void RainDetected::alarm1Interrupt(Water *water)
{
    water->setState(Idle::getInstance());
}

WaterState &RainDetected::getInstance()
{
    static RainDetected instance;
    return instance;
}