#include <jled.h>
#include <DS3231.h>
#include <EEPROM.h>

#include "Water.h"
#include "ConcreteWaterStates.h"

Water::Water(JLed *led, DS3231 *rtc)
{
    this->led = led;
    this->rtc = rtc;

    this->watering = false;

    // Read EEPROM Settings if exists
    if (EEPROM.read(128) == 'S')
    { // Read water length from EEPROM
        EEPROM.get(129, waterLength);
    }
    else
    { // Set default water length
        waterLength = 10;
        EEPROM.put(128, 'S');
        EEPROM.put(129, waterLength);
    }

    this->waterLength = waterLength;
    this->waterStartMinute = 0;

    currentState = previousState = &Idle::getInstance();
    currentState->enter(this);
}

void Water::setState(WaterState &newState)
{
    currentState->exit(this);
    previousState = currentState;
    currentState = &newState;
    currentState->enter(this);
}

void Water::execute()
{
    currentState->execute(this);
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