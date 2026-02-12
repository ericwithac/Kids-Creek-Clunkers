#include <Modulino.h>

ModulinoDistance distanceSensor;
ModulinoBuzzer buzzer;
ModulinoButtons buttons;

const long SERIAL_BAUD = 9600;
bool lastPressed[3] = {false, false, false};
int activeScale = 1;  // 0=Low, 1=Mid, 2=High
unsigned long lastPrintMs = 0;
unsigned long lastToneRefreshMs = 0;

float filteredMm = 220.0f;
bool filterReady = false;

int stableNoteIndex = 0;
int pendingNoteIndex = 0;
int pendingCount = 0;
int lastPitchHz = -1;

const int lowScale[] = {131, 147, 165, 196, 220, 262, 294, 392};
const int midScale[] = {262, 294, 330, 392, 440, 523, 587, 784};
const int highScale[] = {1047, 1175, 1319, 1568, 1760, 2093, 2349, 2637};
const int noteCount = 8;

const char *scaleNames[] = {"Low", "Mid", "High"};

const int *getScaleNotes(int scaleIndex) {
  if (scaleIndex == 0) {
    return lowScale;
  }
  if (scaleIndex == 1) {
    return midScale;
  }
  return highScale;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  distanceSensor.begin();
  buzzer.begin();
  buttons.begin();

  buttons.setLeds(false, true, false);

  Serial.println("Theremin + scale buttons ready!");
  Serial.println("Button 1 = Low, 2 = Mid, 3 = High.");
  Serial.println("Move hand to change note pitch.");
  Serial.println("Open Serial Monitor at 9600 baud.");
}

void loop() {
  buttons.update();

  bool nowPressed[3] = {
    buttons.isPressed(0),
    buttons.isPressed(1),
    buttons.isPressed(2)
  };

  for (int i = 0; i < 3; i++) {
    if (nowPressed[i] && !lastPressed[i]) {
      activeScale = i;
      pendingCount = 0;
      Serial.print("Scale: ");
      Serial.println(scaleNames[activeScale]);
    }
    lastPressed[i] = nowPressed[i];
  }

  buttons.setLeds(activeScale == 0, activeScale == 1, activeScale == 2);

  if (!distanceSensor.available()) {
    delay(10);
    return;
  }

  int mm = distanceSensor.get();
  int safeMm = constrain(mm, 40, 400);

  if (!filterReady) {
    filteredMm = safeMm;
    filterReady = true;
  }

  // Smooth sensor noise so the sound is steadier.
  filteredMm = (filteredMm * 0.82f) + (safeMm * 0.18f);

  // Far = low note, close = high note.
  int candidateIndex = map((int)filteredMm, 400, 40, 0, noteCount - 1);
  candidateIndex = constrain(candidateIndex, 0, noteCount - 1);

  // Require a few consistent reads before changing note.
  if (candidateIndex != stableNoteIndex) {
    if (candidateIndex == pendingNoteIndex) {
      pendingCount++;
    } else {
      pendingNoteIndex = candidateIndex;
      pendingCount = 1;
    }

    if (pendingCount >= 3) {
      stableNoteIndex = candidateIndex;
      pendingCount = 0;
    }
  } else {
    pendingCount = 0;
  }

  const int *activeNotes = getScaleNotes(activeScale);
  int pitchHz = activeNotes[stableNoteIndex];

  // Overlap tones for a more continuous sound.
  if (pitchHz != lastPitchHz || millis() - lastToneRefreshMs > 40) {
    buzzer.tone(pitchHz, 80);
    lastPitchHz = pitchHz;
    lastToneRefreshMs = millis();
  }

  if (millis() - lastPrintMs > 160) {
    lastPrintMs = millis();
    Serial.print("Raw mm: ");
    Serial.print(mm);
    Serial.print("  Smooth mm: ");
    Serial.print((int)filteredMm);
    Serial.print("  Scale: ");
    Serial.print(scaleNames[activeScale]);
    Serial.print("  Note Hz: ");
    Serial.println(pitchHz);
  }

  delay(12);
}
