#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>
#include <jled.h>
#include <OneButton.h>

#include "Water.h"
#include "WaterState.h"
#include "ConcreteWaterStates.h"

// rtc interrupt pin
#define CLINT 3
// Water valve relay pin
#define VALVE_RELAY_PIN 13
// LED pin
#define LED_PIN 12
// Switch pin
#define SWITCH_PIN 2

// Setup clock
DS3231 rtc;

// LED setup
JLed led = JLed(LED_PIN).LowActive();

// Initializa Water State Machine
Water *state = new Water(&led, &rtc);

// Variables for use in method parameter lists
byte alarmDay;
byte alarmHour = 17;
byte alarmMinute = 53;
byte alarmSecond = 0;
byte alarmBits = 0b00001000; // Alarm 1 every day
bool alarmDayIsDay = false;
bool alarmH12 = false;
bool alarmPM = false;

// SWitch setup
OneButton button(SWITCH_PIN, true);

// Interrupt signaling byte
volatile byte alarm1Interrupt = 0;

void isr_Alarm1()
{
  // interrupt signals to loop
  alarm1Interrupt = 1;
  return;
}

void setup()
{
  // Begin I2C communication
  Wire.begin();

  // Begin Serial communication
  Serial.begin(9600);

  // Set alarm 1
  rtc.turnOffAlarm(1);
  rtc.setA1Time(
      alarmDay, alarmHour, alarmMinute, alarmSecond,
      alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  // enable Alarm 1 interrupts
  rtc.turnOnAlarm(1);
  // clear Alarm 1 flag
  rtc.checkIfAlarm(1);

  // Link the button functions
  button.attachClick(
      []()
      {
        Serial.println("Button click");
        state->buttonShortPress();
      });

  button.attachDoubleClick(
      []()
      {
        Serial.println("Button double click");
        state->buttonDoublePress();
      });

  button.attachLongPressStart(
      []()
      {
        Serial.println("Button long press start");
        state->buttonLongPress();
      });

  // Attach clock interrupt
  pinMode(CLINT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLINT), isr_Alarm1, FALLING);

  // Define relay pin
  pinMode(VALVE_RELAY_PIN, OUTPUT);
}

void checkAlarm()
{
  // Check if alarm 1 has triggered
  if (alarm1Interrupt)
  {
    alarm1Interrupt = 0;
    Serial.println("Alarm 1 triggered");
    state->setState(Watering::getInstance());
  }
}

void handleValve()
{
  // Set valve relay active low when watering
  digitalWrite(VALVE_RELAY_PIN, !state->isWatering());
}

bool stateUpdated = false;

void handleCurrentState()
{
  // prevent from triggering twice in the same second
  if (rtc.getSecond() % 5 == 0 && !stateUpdated)
  {
    state->execute();
    handleValve();
    stateUpdated = true;
  }
  else if (rtc.getSecond() % 5 != 0)
  {
    stateUpdated = false;
  }
}

void loop()
{
  checkAlarm();

  button.tick();
  led.Update();

  handleCurrentState();
}