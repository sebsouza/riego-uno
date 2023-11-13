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

// SWitch setup
OneButton button(SWITCH_PIN, true);

// Initializa Water State Machine
Water *state = new Water(&led, &rtc, &button);

// Variables for use in method parameter lists
byte alarmDay;
byte alarmHour = 22;
byte alarmMinute = 45;
byte alarmSecond = 0;
byte alarmBits = 0b00001000; // Alarm 1 every day
bool alarmDayIsDay = false;
bool alarmH12 = false;
bool alarmPM = false;

// Interrupt signaling byte
volatile byte alarm1Interrupt = 0;

void isr_Alarm1()
{
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

  alarmMinute = 0xFF;     // a value that will never match the time
  alarmBits = 0b01100000; // Alarm 2 when minutes match, i.e., never

  // Upload the parameters to prevent Alarm 2 entirely
  rtc.setA2Time(
      alarmDay, alarmHour, alarmMinute,
      alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  // disable Alarm 2 interrupt
  rtc.turnOffAlarm(2);
  // clear Alarm 2 flag
  rtc.checkIfAlarm(2);

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

void handleValve()
{
  // Set valve relay active low when watering
  digitalWrite(VALVE_RELAY_PIN, !state->isWatering());
}

void checkAlarms()
{
  rtc.checkIfAlarm(1);
  if (alarm1Interrupt)
  {
    alarm1Interrupt = 0;
    Serial.println("Alarm 1 interrupt");
    state->alarm1Interrupt();
  }
}

void loop()
{
  checkAlarms();
  state->execute();

  handleValve();
}