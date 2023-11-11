#include "ConcreteWaterStates.h"

void blinkWaterLength(Water *water)
{
    byte currentWaterLength = water->getWaterLength();
    byte lengthUnit = currentWaterLength % 10;
    byte lengthTens = currentWaterLength / 10;

    Serial.print("lengthTens: ");
    Serial.println(lengthTens);
    Serial.print("lengthUnit: ");
    Serial.println(lengthUnit);

    water->useLed()->Off().Update();

    water->useLed()->DelayBefore(500).Blink(500, 100).Repeat(lengthTens).Update();
    do
    {
        water->useLed()->Update();
    } while (water->useLed()->Update());

    water->useLed()->Blink(200, 100).Repeat(lengthUnit).Update();
}

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
}

void Idle::buttonDoublePress(Water *water)
{
    water->setState(RainDetected::getInstance());
}

void Idle::buttonLongPress(Water *water)
{
    water->setState(Settings::getInstance());
}

WaterState &Idle::getInstance()
{
    static Idle instance;
    return instance;
}

// Watering implementation

void Watering::enter(Water *water)
{
    Serial.println("Watering::enter");
    water->setWatering(true);
    water->setWaterStartMinute(water->getCurrentMinute());
    water->useLed()->Blink(1000, 200).Forever();
}

void Watering::execute(Water *water)
{
}

void Watering::exit(Water *water)
{
    water->setWatering(false);
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

WaterState &Watering::getInstance()
{
    static Watering instance;
    return instance;
}

// Settings implementation

void Settings::enter(Water *water)
{
    blinkWaterLength(water);
    Serial.println("Settings::enter");
}

void Settings::execute(Water *water)
{
}

void Settings::exit(Water *water)
{
}

void Settings::buttonShortPress(Water *water)
{
    water->setWaterLength(water->getWaterLength() + 1);
    blinkWaterLength(water);
}

void Settings::buttonDoublePress(Water *water)
{
    water->setWaterLength(water->getWaterLength() - 1);
    blinkWaterLength(water);
}

void Settings::buttonLongPress(Water *water)
{
    water->setState(Idle::getInstance());
}

WaterState &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

// RainDetected implementation

void RainDetected::enter(Water *water)
{
    water->useLed()->Blink(100, 3000).Forever();
    Serial.println("RainDetected::enter");
}

void RainDetected::execute(Water *water)
{
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

WaterState &RainDetected::getInstance()
{
    static RainDetected instance;
    return instance;
}