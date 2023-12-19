#include "Water.h"
#include "ConcreteWaterStates.h"

#define LED_BLINK_TIME_ON_SHORT 100
#define LED_BLINK_TIME_ON_LONG 400
#define LED_BLINK_TIME_OFF 100

void wait(Water *water)
{
    do
    {
        water->useLed()->Update();
        water->useButton()->tick();

    } while (water->useLed()->Update());
}

void waitOff(Water *water, int milliseconds)
{
    water->useLed()->Off(milliseconds).Update();
    wait(water);
}

void blinkTens(Water *water, byte tens)

{
    if (tens > 0)
    {
        waitOff(water, LED_BLINK_TIME_OFF);
        water->useLed()->Blink(LED_BLINK_TIME_ON_LONG, LED_BLINK_TIME_OFF).Repeat(tens).Update();
        wait(water);
        waitOff(water, LED_BLINK_TIME_OFF);
    }
}

void blinkUnits(Water *water, byte units)
{
    if (units > 0)
    {
        waitOff(water, LED_BLINK_TIME_OFF);
        water->useLed()->Blink(LED_BLINK_TIME_ON_SHORT, LED_BLINK_TIME_OFF).Repeat(units).Update();
        wait(water);
        waitOff(water, LED_BLINK_TIME_OFF);
    }
}

void blinkRemainingTime(Water *water)
{
    byte currentMinute = water->getCurrentMinute();
    byte currentSecond = water->getCurrentSecond();
    byte waterStartMinute = water->getWaterStartMinute();
    byte waterStartSecond = water->getWaterStartSecond();
    byte waterLength = water->getWaterLength();

    int remainingSeconds = (waterStartMinute * 60 + waterStartSecond + waterLength * 60) - (currentMinute * 60 + currentSecond);
    byte remainingMinutes = remainingSeconds / 60 + 1;

    byte remainingTimeUnit = remainingMinutes % 10;
    byte remainingTimeTens = remainingMinutes / 10;

    blinkTens(water, remainingTimeTens);
    waitOff(water, LED_BLINK_TIME_OFF);

    blinkUnits(water, remainingTimeUnit);
    waitOff(water, LED_BLINK_TIME_OFF);
    water->useLed()->On().Update();
}

void checkWateringTime(Water *water)
{
    byte currentMinute = water->getCurrentMinute();
    byte currentSecond = water->getCurrentSecond();
    byte waterStartSecond = water->getWaterStartSecond();
    byte waterStartMinute = water->getWaterStartMinute();
    byte waterLength = water->getWaterLength();

    if (currentMinute * 60 + currentSecond - waterStartMinute * 60 - waterStartSecond >= waterLength * 60)
    {
        water->setWatering(false);
        if (water->getCurrentState() == &Watering::getInstance())
        {
            water->setState(Idle::getInstance());
        }
        Serial.println("Watering timer triggered");
    }
}

void blinkWaterLength(Water *water)
{
    byte currentWaterLength = water->getWaterLength();
    byte lengthUnit = currentWaterLength % 10;
    byte lengthTens = currentWaterLength / 10;

    blinkTens(water, lengthTens);
    blinkUnits(water, lengthUnit);
}

void blinkTemperatureThreshold(Water *water)
{
    byte temperatureThreshold = water->getTemperatureThreshold();
    byte temperatureUnit = temperatureThreshold % 10;
    byte temperatureTens = temperatureThreshold / 10;

    blinkTens(water, temperatureTens);
    blinkUnits(water, temperatureUnit);
}

void blinkAlarm(Water *water)
{
    byte alarmTime = water->getAlarmTime();

    blinkUnits(water, alarmTime + 1);
}

bool isHot(Water *water)
{
    float currentTemperature = water->getCurrentTemperature();
    byte temperatureThreshold = water->getTemperatureThreshold();

    Serial.print("Current temperature: ");
    Serial.println(currentTemperature);
    Serial.print("Temperature threshold: ");
    Serial.println(temperatureThreshold);

    if (currentTemperature > temperatureThreshold)
    {
        Serial.println("Temperature threshold triggered");
        return true;
    }

    return false;
}

void handleSetupAlarmInterrupt(Water *water)
{
    WaterState *previousState = water->getPreviousState();
    if (previousState == &Idle::getInstance())
    {
        water->setState(Watering::getInstance());
    }
    else
    {
    }
}