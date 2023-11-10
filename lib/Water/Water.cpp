#include <jled.h>
#include <DS3231.h>

#include "Water.h"
#include "ConcreteWaterStates.h"

Water::Water(JLed *led, DS3231 *rtc, byte waterLength)
{
    this->led = led;
    this->rtc = rtc;

    this->waterLength = waterLength;

    currentState = &WaterOff::getInstance();
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

// void Water::buttonTriplePress()
// {
//     currentState->buttonTriplePress(this);
// }

void Water::buttonLongPress()
{
    currentState->buttonLongPress(this);
}