#include "Water.h"
#include "ConcreteWaterStates.h"

#define LED_BLINK_TIME_ON_SHORT 100
#define LED_BLINK_TIME_ON_LONG 400
#define LED_BLINK_TIME_OFF 150

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
    }
    water->useLed()->Blink(LED_BLINK_TIME_ON_LONG, LED_BLINK_TIME_OFF).Repeat(tens).Update();
    wait(water);
}

void blinkUnits(Water *water, byte units)
{
    if (units > 0)
    {
        waitOff(water, LED_BLINK_TIME_OFF);
    }
    water->useLed()->Blink(LED_BLINK_TIME_ON_SHORT, LED_BLINK_TIME_OFF).Repeat(units).Update();
    wait(water);
}

void blinkWaterLength(Water *water)
{
    byte currentWaterLength = water->getWaterLength();
    byte lengthUnit = currentWaterLength % 10;
    byte lengthTens = currentWaterLength / 10;

    blinkTens(water, lengthTens);
    blinkUnits(water, lengthUnit);
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
    blinkUnits(water, remainingTimeUnit);
    waitOff(water, LED_BLINK_TIME_OFF);
    water->useLed()->On().Update();

    Serial.println("Blink remaining time _________________________________");

    Serial.print("Current time: ");
    Serial.print(currentMinute);
    Serial.print(":");
    Serial.println(currentSecond);

    Serial.print("Water start time: ");
    Serial.print(waterStartMinute);
    Serial.print(":");
    Serial.println(waterStartSecond);

    Serial.print("Water length: ");
    Serial.println(waterLength);

    Serial.print("Remaining minutes: ");
    Serial.println(remainingMinutes);
}

void checkWateringTime(Water *water)
{
    byte currentMinute = water->getCurrentMinute();
    byte currentSecond = water->getCurrentSecond();
    byte waterStartSecond = water->getWaterStartSecond();
    byte waterStartMinute = water->getWaterStartMinute();
    byte waterLength = water->getWaterLength();

    // Serial.print("Current minute: ");
    // Serial.println(currentMinute);
    // Serial.print("Water start minute: ");
    // Serial.println(waterStartMinute);
    // Serial.print("Water length: ");
    // Serial.println(waterLength);

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