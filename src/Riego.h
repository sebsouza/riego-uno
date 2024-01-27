#define CLINT 3
#define VALVE_RELAY_PIN 13
#define LED_PIN 12
#define SWITCH_PIN 2
#define BUZZER_PIN 9

// Alarm 1 setup
byte alarm1Day;
byte alarm1Hour = 6;
byte alarm1Minute = 0;
byte alarm1Second = 0;
byte alarm1Bits = 0b00001000; // Alarm 1 every day
bool alarm1DayIsDay = false;
bool alarm1H12 = false;
bool alarm1PM = false;

// Alarm 2 setup1
byte alarm2Day;
byte alarm2Hour = 13;
byte alarm2Minute = 0;
byte alarm2Bits = 0b01000000; // Alarm 2 every day
bool alarm2DayIsDay = false;
bool alarm2H12 = false;
bool alarm2PM = false;

// Interrupt signaling byte
volatile byte alarmInterrupt = 0;