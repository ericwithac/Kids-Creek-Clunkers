#include <Modulino.h>

ModulinoThermo thermo;
ModulinoPixels pixels;

const int ledCount = 8;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  thermo.begin();
  pixels.begin();

  Serial.println("Heat bar: warmer = more red LEDs.");
}

void loop() {
  float tempC = thermo.getTemperature();

  // Kids-friendly range for room demos.
  float tMin = 18.0;
  float tMax = 32.0;
  int lit = map((int)(constrain(tempC, tMin, tMax) * 10), (int)(tMin * 10), (int)(tMax * 10), 0, ledCount);
  lit = constrain(lit, 0, ledCount);

  for (int i = 0; i < ledCount; i++) {
    if (i < lit) {
      // Simple gradient: green to red across the strip.
      int red = map(i, 0, ledCount - 1, 30, 255);
      int green = map(i, 0, ledCount - 1, 255, 20);
      pixels.set(i, ModulinoColor(red, green, 0), 18);
    } else {
      pixels.set(i, ModulinoColor(0, 0, 0), 0);
    }
  }
  pixels.show();

  Serial.print("Temp C: ");
  Serial.print(tempC, 1);
  Serial.print("  Lit LEDs: ");
  Serial.println(lit);

  delay(300);
}

