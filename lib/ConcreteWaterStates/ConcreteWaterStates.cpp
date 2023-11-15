#include <EEPROM.h>
#include "ConcreteWaterStates.h"
#include "services.cpp"

// Idle implementation

void Idle::enter(Water *water)
{
    water->setWatering(false);
    water->useLed()->On().Update();

    Serial.println("Idle::enter");
}

void Idle::execute(Water *water)
{
}

void Idle::exit(Water *water)
{
    WaterState &idleInstance = Idle::getInstance();
    water->setPreviousState(idleInstance);
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
    water->setState(LengthSetup::getInstance());
}

void Idle::alarm1Interrupt(Water *water)
{
    water->setState(Watering::getInstance());
}

void Idle::alarm2Interrupt(Water *water)
{
    if (isHot(water))
    {
        water->setState(Watering::getInstance());
    }
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
    WaterState &wateringInstance = Watering::getInstance();
    water->setPreviousState(wateringInstance);
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
    water->setState(LengthSetup::getInstance());
}

void Watering::alarm1Interrupt(Water *water)
{
}

void Watering::alarm2Interrupt(Water *water)
{
}

WaterState &Watering::getInstance()
{
    static Watering instance;
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
    WaterState &rainDetectedInstance = RainDetected::getInstance();
    water->setPreviousState(rainDetectedInstance);
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
    water->setState(LengthSetup::getInstance());
}

void RainDetected::alarm1Interrupt(Water *water)
{
    water->setState(Idle::getInstance());
}

void RainDetected::alarm2Interrupt(Water *water)
{
}

WaterState &RainDetected::getInstance()
{
    static RainDetected instance;
    return instance;
}

// LengthSetup implementation

void LengthSetup::enter(Water *water)
{
    water->useLed()->Off().Update();

    Serial.println("LengthSetup::enter");
}

void LengthSetup::execute(Water *water)
{
    blinkWaterLength(water);

    if (water->isWatering())
    {
        checkWateringTime(water);
    }
}

void LengthSetup::exit(Water *water)
{
    EEPROM.put(128, 'S');
    EEPROM.put(129, water->getWaterLength());
}

void LengthSetup::buttonShortPress(Water *water)
{
    water->setWaterLength(water->getWaterLength() + 1);
}

void LengthSetup::buttonDoublePress(Water *water)
{
    water->setWaterLength(water->getWaterLength() - 1);
}

void LengthSetup::buttonLongPress(Water *water)
{
    water->setState(TemperatureSetup::getInstance());
}

void LengthSetup::alarm1Interrupt(Water *water)
{
    handleSetupAlarmInterrupt(water);
}

void LengthSetup::alarm2Interrupt(Water *water)
{
    if (isHot(water))
    {
        handleSetupAlarmInterrupt(water);
    }
}

WaterState &LengthSetup::getInstance()
{
    static LengthSetup instance;
    return instance;
}

// TemperatureSetup implementation

void TemperatureSetup::enter(Water *water)
{
    water->useLed()->Off().Update();

    Serial.println("TemperatureSetup::enter");
}

void TemperatureSetup::execute(Water *water)
{
    blinkTemperatureThreshold(water);

    if (water->isWatering())
    {
        checkWateringTime(water);
    }
}

void TemperatureSetup::exit(Water *water)
{
    EEPROM.put(130, water->getTemperatureThreshold());
}

void TemperatureSetup::buttonShortPress(Water *water)
{
    water->setTemperatureThreshold(water->getTemperatureThreshold() + 1);
}

void TemperatureSetup::buttonDoublePress(Water *water)
{
    water->setTemperatureThreshold(water->getTemperatureThreshold() - 1);
}

void TemperatureSetup::buttonLongPress(Water *water)
{
    WaterState *previousState = water->getPreviousState();
    water->setState(*previousState);
}

void TemperatureSetup::alarm1Interrupt(Water *water)
{
    handleSetupAlarmInterrupt(water);
}

void TemperatureSetup::alarm2Interrupt(Water *water)
{
    if (isHot(water))
    {
        handleSetupAlarmInterrupt(water);
    }
}

WaterState &TemperatureSetup::getInstance()
{
    static TemperatureSetup instance;
    return instance;
}
