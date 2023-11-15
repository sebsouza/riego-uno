#include <jled.h>
#include <DS3231.h>
#include <EEPROM.h>

#include "Water.h"
#include "ConcreteWaterStates.h"

#define REFRESH_TIME_SECONDS 6

Water::Water(JLed *led, DS3231 *rtc, OneButton *button)
{
    this->led = led;
    this->rtc = rtc;
    this->button = button;

    this->watering = false;

    // Read EEPROM Settings if exists
    if (EEPROM.read(128) == 'S')
    { // Read water length from EEPROM
        EEPROM.get(129, waterLength);
        EEPROM.get(130, temperatureThreslhold);
        temperatureThreslhold = 35;
    }
    else
    { // Set default water length
        waterLength = 10;
        temperatureThreslhold = 35;

        EEPROM.put(128, 'S');
        EEPROM.put(129, waterLength);
        EEPROM.put(130, temperatureThreslhold);
    }

    this->waterLength = waterLength;
    this->waterStartMinute = 0;
    this->temperatureThreslhold = temperatureThreslhold;

    currentState = previousState = &Idle::getInstance();
    currentState->enter(this);
}

void Water::setState(WaterState &newState)
{
    currentState->exit(this);
    currentState = &newState;
    currentState->enter(this);
}

void Water::execute()
{
    if ((this->rtc->getSecond() % REFRESH_TIME_SECONDS == 0) && (!this->stateUpdated))
    {
        this->stateUpdated = true;
        currentState->execute(this);

        bool h12Flag;
        bool pmFlag;

        Serial.print("Current Time: ");
        Serial.print(this->rtc->getHour(h12Flag, pmFlag));
        Serial.print(":");
        Serial.print(this->rtc->getMinute());
        Serial.print(":");
        Serial.println(this->rtc->getSecond());
    }
    else if ((this->rtc->getSecond() % REFRESH_TIME_SECONDS != 0) && (this->stateUpdated))
    {
        this->stateUpdated = false;
    }
    this->led->Update();
    this->button->tick();
}

void Water::buttonShortPress()
{
    currentState->buttonShortPress(this);
}

void Water::buttonDoublePress()
{
    currentState->buttonDoublePress(this);
}

void Water::buttonLongPress()
{
    currentState->buttonLongPress(this);
}

void Water::alarm1Interrupt()
{
    currentState->alarm1Interrupt(this);
}

void Water::alarm2Interrupt()
{
    currentState->alarm2Interrupt(this);
}