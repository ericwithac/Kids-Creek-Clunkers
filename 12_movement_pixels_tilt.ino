#include <Modulino.h>

ModulinoMovement movement;
ModulinoPixels pixels;

const int ledCount = 8;

void setup() {
  Modulino.begin();
  movement.begin();
  pixels.begin();
}

void loop() {
  movement.update();

  float x = movement.getX();             // Left/right tilt info.
  int center = map((int)(x * 100), -100, 100, 0, ledCount - 1);
  center = constrain(center, 0, ledCount - 1);

  for (int i = 0; i < ledCount; i++) {
    int distanceFromCenter = abs(i - center);
    if (distanceFromCenter == 0) {
      pixels.set(i, ModulinoColor(255, 80, 0), 24);   // bright orange
    } else if (distanceFromCenter == 1) {
      pixels.set(i, ModulinoColor(0, 160, 255), 12);  // blue side glow
    } else {
      pixels.set(i, ModulinoColor(0, 0, 0), 0);
    }
  }

  pixels.show();
  delay(35);
}

