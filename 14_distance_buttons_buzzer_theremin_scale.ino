#include <Modulino.h>

ModulinoDistance distanceSensor;
ModulinoBuzzer buzzer;
ModulinoButtons buttons;

const long SERIAL_BAUD = 9600;
bool lastPressed[3] = {false, false, false};
int activeScale = 0;  // 0=Major, 1=Minor, 2=Blues
unsigned long lastPrintMs = 0;

const int majorScale[] = {262, 294, 330, 349, 392, 440, 494, 523, 587};
const int minorScale[] = {262, 294, 311, 349, 392, 415, 466, 523, 587};
const int bluesScale[] = {262, 311, 349, 370, 392, 466, 523, 587, 698};
const int noteCount = 9;

const char *scaleNames[] = {"Major", "Minor", "Blues"};

const int *getScaleNotes(int scaleIndex) {
  if (scaleIndex == 1) {
    return minorScale;
  }
  if (scaleIndex == 2) {
    return bluesScale;
  }
  return majorScale;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  distanceSensor.begin();
  buzzer.begin();
  buttons.begin();

  buttons.setLeds(true, false, false);

  Serial.println("Theremin + scale buttons ready!");
  Serial.println("Button 1 = Major, 2 = Minor, 3 = Blues.");
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

  // Far = low note, close = high note.
  int noteIndex = map(safeMm, 400, 40, 0, noteCount - 1);
  noteIndex = constrain(noteIndex, 0, noteCount - 1);

  const int *activeNotes = getScaleNotes(activeScale);
  int pitchHz = activeNotes[noteIndex];

  buzzer.tone(pitchHz, 35);

  if (millis() - lastPrintMs > 120) {
    lastPrintMs = millis();
    Serial.print("Distance mm: ");
    Serial.print(mm);
    Serial.print("  Scale: ");
    Serial.print(scaleNames[activeScale]);
    Serial.print("  Note Hz: ");
    Serial.println(pitchHz);
  }

  delay(20);
}

