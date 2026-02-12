#include <Modulino.h>

ModulinoBuzzer buzzer;

const int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int noteCount = sizeof(notes) / sizeof(notes[0]);
int noteIndex = 0;

void setup() {
  Modulino.begin();
  buzzer.begin();
}

void loop() {
  buzzer.tone(notes[noteIndex], 220);
  delay(280);

  noteIndex++;
  if (noteIndex >= noteCount) {
    noteIndex = 0;
    delay(400);
  }
}

