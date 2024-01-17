
#ifndef BUZZER_H
#define BUZZER_H

#include "pitches.h"

class Buzzer
{
public:
    Buzzer(byte pin) : pin(pin){};
    void setup();

    void beep();
    void beep(byte times);
    void beep(byte times, unsigned int delayTime);
    void beep(byte times, unsigned int delayTime, unsigned int beepTime);
    void beepLo();
    void longBeep() { beep(1, 0, 300); };
    void playMelody(int melody[], int noteDurations[], int noteCount, int tempo);
    void playXmasMelody();
    void playStarwarsMelody();
    void playGodfatherMelody();

private:
    byte pin;
};

#endif
