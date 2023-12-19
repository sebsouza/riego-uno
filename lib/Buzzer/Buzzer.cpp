#include "Arduino.h"
#include "Buzzer.h"

#define BEEP_NOTE NOTE_C5

void Buzzer::setup()
{
    pinMode(pin, OUTPUT);
}

void Buzzer::beep()
{
    tone(pin, BEEP_NOTE, 100);
}

void Buzzer::beep(byte times)
{
    for (byte i = 0; i < times; i++)
    {
        tone(pin, BEEP_NOTE, 100);
        delay(200);
    }
}

void Buzzer::beep(byte times, unsigned int delayTime)
{
    for (byte i = 0; i < times; i++)
    {
        tone(pin, BEEP_NOTE, 100);
        delay(delayTime);
    }
}

void Buzzer::beep(byte times, unsigned int delayTime, unsigned int beepTime)
{
    for (byte i = 0; i < times; i++)
    {
        tone(pin, BEEP_NOTE, beepTime);
        delay(delayTime);
    }
}

void Buzzer::beepLo()
{
    tone(pin, NOTE_C4, 100);
}

void Buzzer::playMelody(int *melody, int *noteDurations, int noteCount, int bpm)
{
    int noteLength = 60000 / bpm * 2;

    for (int thisNote = 0; thisNote < noteCount; thisNote++)
    {
        int noteDuration;
        if (noteDurations[thisNote] > 0)
        {
            noteDuration = noteLength / noteDurations[thisNote];
        }
        else if (noteDurations[thisNote] < 0) // Dotted note
        {
            noteDuration = noteLength / (noteDurations[thisNote]) * -1.5;
        }
        tone(pin, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(pin);
    }
}

void Buzzer::playXmasMelody()
{
    int *melody = new int[26]{
        NOTE_E5, NOTE_E5, NOTE_E5,
        NOTE_E5, NOTE_E5, NOTE_E5,
        NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
        NOTE_E5,
        NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
        NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
        NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
        NOTE_D5, NOTE_G5};

    int *noteDurations = new int[26]{
        8, 8, 4,
        8, 8, 4,
        8, 8, 8, 8,
        2,
        8, 8, 8, 8,
        8, 8, 8, 16, 16,
        8, 8, 8, 8,
        4, 4};

    playMelody(melody, noteDurations, 26, 150);
    delete[] melody;
    delete[] noteDurations;
}

void Buzzer::playStarwarsMelody()
{
    int *melody = new int[47]{
        NOTE_AS4,
        NOTE_AS4,
        NOTE_AS4, // 1
        NOTE_F5,
        NOTE_C6,
        NOTE_AS5,
        NOTE_A5,
        NOTE_G5,
        NOTE_F6,
        NOTE_C6,
        NOTE_AS5,
        NOTE_A5,
        NOTE_G5,
        NOTE_F6,
        NOTE_C6,
        NOTE_AS5,
        NOTE_A5,
        NOTE_AS5,
        NOTE_G5,
        NOTE_C5,
        NOTE_C5,
        NOTE_C5,
        NOTE_F5,
        NOTE_C6,
        NOTE_AS5,
        NOTE_A5,
        NOTE_G5,
        NOTE_F6,
        NOTE_C6,
        NOTE_AS5,
        NOTE_A5,
        NOTE_G5,
        NOTE_F6,
        NOTE_C6, // 8
        NOTE_AS5,
        NOTE_A5,
        NOTE_AS5,
        NOTE_G5,
    };

    int *noteDurations = new int[47]{
        8,
        8,
        8, // 1
        2,
        2,
        8,
        8,
        8,
        2,
        4,
        8,
        8,
        8,
        2,
        4,
        8,
        8,
        8,
        2,
        8,
        8,
        8,
        2,
        2,
        8,
        8,
        8,
        2,
        4,
        8,
        8,
        8,
        2,
        4, // 8
        8,
        8,
        8,
        2,
    };

    playMelody(melody, noteDurations, 47, 90);
    delete[] melody;
    delete[] noteDurations;
}

void Buzzer::playGodfatherMelody()
{
    // The Godfather theme
    // Score available at https://musescore.com/user/35463/scores/55160
    int *melody = new int[44]{

        NOTE_E4,
        NOTE_A4,
        NOTE_C5,
        NOTE_B4,
        NOTE_A4,
        NOTE_C5,
        NOTE_A4,
        NOTE_B4,
        NOTE_A4,
        NOTE_F4,
        NOTE_G4,
        NOTE_E4,
        NOTE_E4,
        NOTE_A4,
        NOTE_C5,
        NOTE_B4,
        NOTE_A4,
        NOTE_C5,
        NOTE_A4,
        NOTE_C5,
        NOTE_A4,
        NOTE_E4,
        NOTE_DS4,

        NOTE_D4,
        NOTE_D4,
        NOTE_F4,
        NOTE_GS4,
        NOTE_B4,
        NOTE_D4,
        NOTE_F4,
        NOTE_GS4,
        NOTE_A4,
        NOTE_C4,
        NOTE_C4,
        NOTE_G4,
        NOTE_F4,
        NOTE_E4,
        NOTE_G4,
        NOTE_F4,
        NOTE_F4,
        NOTE_E4,
        NOTE_E4,
        NOTE_GS4,

        NOTE_A4,

    };

    int *noteDurations = new int[44]{

        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        2,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,

        2,
        8,
        8,
        8,
        2,
        8,
        8,
        8,
        2,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,

        2,
    };

    playMelody(melody, noteDurations, 44, 90);
    delete[] melody;
    delete[] noteDurations;
}