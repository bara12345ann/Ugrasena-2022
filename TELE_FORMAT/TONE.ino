#include "pitches.h"
int buzzerPin = 2;

int melody[] = {
  NOTE_E5, NOTE_E5 , NOTE_E5, NOTE_E5 , NOTE_E5, NOTE_E5
};

int noteDurations[] = {
  8, 6, 8, 6, 8, 6
};

void initBuzzer() {
  for (int thisNote = 0; thisNote < sizeof(melody) / 2; thisNote++) {
    int noteDuration = 2000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}
