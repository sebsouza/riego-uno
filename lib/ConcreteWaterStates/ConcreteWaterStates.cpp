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

    water->useLed()->DelayBefore(500).Blink(300, 100).Repeat(lengthTens);
    do
    {
        water->useLed()->Update();
    } while (water->useLed()->Update());

    water->useLed()->Blink(100, 100).Repeat(lengthUnit).Update();
}

// WaterOff implementation

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

void WaterOff::buttonDoublePress(Water *water)
{
    water->setRainDetected(!water->isRainDetected());
    Serial.print("Rain detected: ");
    Serial.println(water->isRainDetected() ? 'true' : 'false');
    water->useLed()->Blink(100, 100).Repeat(water->isRainDetected() ? 1 : 0);
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

// WaterOn implementation

void WaterOn::enter(Water *water)
{
    water->setWatering(true);
    water->setWaterStartMinute(water->getCurrentMinute());
    water->useLed()->Blink(1000, 200).Forever();
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

void WaterOn::buttonDoublePress(Water *water)
{
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

// WaterConfig implementation

void WaterConfig::enter(Water *water)
{
    blinkWaterLength(water);
}

void WaterConfig::execute(Water *water)
{
}

void WaterConfig::exit(Water *water)
{
}

void WaterConfig::buttonShortPress(Water *water)
{
    water->setWaterLength(water->getWaterLength() + 1);
    blinkWaterLength(water);
}

void WaterConfig::buttonDoublePress(Water *water)
{
    water->setWaterLength(water->getWaterLength() - 1);
    blinkWaterLength(water);
}

void WaterConfig::buttonLongPress(Water *water)
{
    water->setState(WaterOff::getInstance());
}

WaterState &WaterConfig::getInstance()
{
    static WaterConfig instance;
    return instance;
}