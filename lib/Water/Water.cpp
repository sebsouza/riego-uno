#include <jled.h>
#include <DS3231.h>
#include <EEPROM.h>
#include "Buzzer.h"

#include "Water.h"
#include "ConcreteWaterStates.h"

#define REFRESH_TIME_SECONDS 6

Water::Water(JLed *led, DS3231 *rtc, OneButton *button, Buzzer *buzzer)
{
    this->led = led;
    this->rtc = rtc;
    this->button = button;
    this->buzzer = buzzer;

    this->watering = false;

    // Read EEPROM Settings if exists
    if (EEPROM.read(128) == 'S')
    { // Read from EEPROM
        EEPROM.get(129, waterLength);
        EEPROM.get(130, temperatureThreslhold);
        EEPROM.get(131, alarmTime);
    }
    else
    { // Set default parameters
        waterLength = 10;
        temperatureThreslhold = 35;
        alarmTime = 0;

        EEPROM.put(128, 'S');
        EEPROM.put(129, waterLength);
        EEPROM.put(130, temperatureThreslhold);
        EEPROM.put(131, alarmTime);
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