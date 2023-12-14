#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>
#include <jled.h>
#include <OneButton.h>

#include "Riego.h"
#include "Water.h"
#include "WaterState.h"
#include "ConcreteWaterStates.h"

DS3231 rtc;
JLed led = JLed(LED_PIN).LowActive();
OneButton button(SWITCH_PIN, true);
Buzzer buzzer(BUZZER_PIN);

Water *state = new Water(&led, &rtc, &button, &buzzer);

void isr_Alarm()
{
  alarmInterrupt = 1;

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
      alarm1Day, alarm1Hour, alarm1Minute, alarm1Second,
      alarm1Bits, alarm1DayIsDay, alarm1H12, alarm1PM);
  // enable Alarm 1 interrupts
  rtc.turnOnAlarm(1);
  // clear Alarm 1 flag
  rtc.checkIfAlarm(1);

  // Set alarm 2
  rtc.turnOffAlarm(2);
  rtc.setA2Time(
      alarm2Day, alarm2Hour, alarm2Minute,
      alarm2Bits, alarm2DayIsDay, alarm2H12, alarm2PM);
  // enable Alarm 2 interrupts
  rtc.turnOnAlarm(2);
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

  pinMode(VALVE_RELAY_PIN, OUTPUT);
  pinMode(CLINT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLINT), isr_Alarm, FALLING);
}

void handleValve()
{
  // Set valve relay active low when watering
  digitalWrite(VALVE_RELAY_PIN, !state->isWatering());
}

void checkAlarms()
{
  if (alarmInterrupt)
  {
    alarmInterrupt = 0;
    if (rtc.checkIfAlarm(1))
    {
      Serial.println("Alarm 1 interrupt");
      state->alarm1Interrupt();
    }
    if (rtc.checkIfAlarm(2))
    {
      Serial.println("Alarm 2 interrupt");
      state->alarm2Interrupt();
    }
  }
}

void loop()
{
  checkAlarms();
  state->execute();

  handleValve();
}