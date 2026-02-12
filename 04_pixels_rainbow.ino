#include <Modulino.h>

ModulinoPixels pixels;

ModulinoColor rainbow[] = {
  ModulinoColor(255, 0, 0),    // red
  ModulinoColor(255, 80, 0),   // orange
  ModulinoColor(255, 180, 0),  // yellow
  ModulinoColor(0, 255, 0),    // green
  ModulinoColor(0, 130, 255),  // blue
  ModulinoColor(150, 0, 255)   // purple
};

const int ledCount = 8;
int offset = 0;

void setup() {
  Modulino.begin();
  pixels.begin();
}

void loop() {
  for (int i = 0; i < ledCount; i++) {
    int colorIndex = (i + offset) % 6;
    pixels.set(i, rainbow[colorIndex], 12);
  }
  pixels.show();

  offset = (offset + 1) % 6;
  delay(220);
}

