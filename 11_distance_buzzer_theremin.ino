#include <Modulino.h>

ModulinoDistance distanceSensor;
ModulinoBuzzer buzzer;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  distanceSensor.begin();
  buzzer.begin();

  Serial.println("Theremin mode: move your hand to change pitch!");
}

void loop() {
  if (!distanceSensor.available()) {
    delay(10);
    return;
  }

  int mm = distanceSensor.get();
  int safeMm = constrain(mm, 40, 400);

  // Closer hand = higher sound.
  int pitchHz = map(safeMm, 400, 40, 180, 1400);

  // Tone length is short so pitch updates quickly.
  buzzer.tone(pitchHz, 35);

  Serial.print("Distance mm: ");
  Serial.print(mm);
  Serial.print("  Pitch Hz: ");
  Serial.println(pitchHz);

  delay(20);
}

