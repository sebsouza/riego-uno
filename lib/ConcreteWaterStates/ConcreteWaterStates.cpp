#include <EEPROM.h>
#include "ConcreteWaterStates.h"

void blinkTens(JLed *led, byte tens)
{
    led->Off().Update();
    led->DelayBefore(300).Blink(500, 150).Repeat(tens).Update();
    do
    {
        led->Update();
    } while (led->Update());
}

void blinkUnits(JLed *led, byte units)
{
    led->Off().Update();
    led->DelayBefore(300).Blink(150, 150).Repeat(units).Update();
    do
    {
        led->Update();
    } while (led->Update());
}

void blinkWaterLength(Water *water)
{
    byte currentWaterLength = water->getWaterLength();
    byte lengthUnit = currentWaterLength % 10;
    byte lengthTens = currentWaterLength / 10;

    blinkTens(water->useLed(), lengthTens);
    blinkUnits(water->useLed(), lengthUnit);
}

void blinkRemainingTime(Water *water)
{
    byte currentMinute = water->getCurrentMinute();
    byte waterStartMinute = water->getWaterStartMinute();
    byte waterLength = water->getWaterLength();

    byte remainingTime = waterLength - (currentMinute - waterStartMinute);

    byte remainingTimeUnit = remainingTime % 10;
    byte remainingTimeTens = remainingTime / 10;

    Serial.print("Remaining time: ");
    Serial.print(remainingTimeTens);
    Serial.print(remainingTimeUnit);
    Serial.println();

    blinkTens(water->useLed(), remainingTimeTens);
    blinkUnits(water->useLed(), remainingTimeUnit);
    water->useLed()->On().Update();
}

void checkWateringTime(Water *water)
{
    byte currentMinute = water->getCurrentMinute();
    byte waterStartSecond = water->getWaterStartSecond();
    byte waterStartMinute = water->getWaterStartMinute();
    byte waterLength = water->getWaterLength();

    Serial.print("Current minute: ");
    Serial.println(currentMinute);
    Serial.print("Water start minute: ");
    Serial.println(waterStartMinute);
    Serial.print("Water length: ");
    Serial.println(waterLength);

    if (currentMinute * 60 - waterStartMinute * 60 - waterStartSecond >= waterLength * 60)
    {
        water->setState(Idle::getInstance());
        Serial.println("Watering timer triggered");
    }
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
    Serial.print("Water length: ");
    Serial.println(water->getWaterLength());

    if (!water->isWatering())
    {
        water->setWatering(true);
        water->setWaterStartMinute(water->getCurrentMinute());
    }

    water->useLed()->Blink(1000, 1000).Forever().Update();
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

WaterState &Watering::getInstance()
{
    static Watering instance;
    return instance;
}

// Settings implementation

void Settings::enter(Water *water)
{
    Serial.println("Settings::enter");
    blinkWaterLength(water);
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
    blinkWaterLength(water);
}

void Settings::buttonDoublePress(Water *water)
{
    water->setWaterLength(water->getWaterLength() - 1);
    blinkWaterLength(water);
}

void Settings::buttonLongPress(Water *water)
{
    water->setState(*(water->getPreviousState()));
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
    water->useLed()->Blink(100, 3000).Forever();
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