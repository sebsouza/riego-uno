#include <jled.h>

#include "Water.h"
#include "ConcreteWaterStates.h"

Water::Water(JLed *led)
{
    currentState = &WaterOff::getInstance();
    currentState->enter(this);
    this->led = led;
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
