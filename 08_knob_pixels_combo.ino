#include <Modulino.h>

ModulinoKnob knob;
ModulinoPixels pixels;

ModulinoColor palette[] = {
  ModulinoColor(255, 0, 0),   // red
  ModulinoColor(0, 255, 0),   // green
  ModulinoColor(0, 120, 255), // blue
  ModulinoColor(255, 170, 0)  // gold
};

const int ledCount = 8;
int colorIndex = 0;
bool pressedLastLoop = false;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  knob.begin();
  pixels.begin();
  knob.set(0);

  Serial.println("Turn knob = LED count. Press knob = change color.");
}

void loop() {
  int rawValue = knob.get();
  int lit = map(constrain(rawValue, 0, 100), 0, 100, 0, ledCount);

  bool pressedNow = knob.isPressed();
  if (pressedNow && !pressedLastLoop) {
    colorIndex = (colorIndex + 1) % 4;
    Serial.print("New color index: ");
    Serial.println(colorIndex);
  }
  pressedLastLoop = pressedNow;

  for (int i = 0; i < ledCount; i++) {
    if (i < lit) {
      pixels.set(i, palette[colorIndex], 16);
    } else {
      pixels.set(i, ModulinoColor(0, 0, 0), 0);
    }
  }
  pixels.show();

  delay(30);
}

