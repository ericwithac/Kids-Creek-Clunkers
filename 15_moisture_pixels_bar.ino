#include <Modulino.h>

ModulinoPixels pixels;

const int SENSOR_PIN = A0;
const int LED_COUNT = 8;
const long SERIAL_BAUD = 9600;

// Calibrate these two numbers for your own sensor + soil.
// Read raw value in Serial Monitor:
// - put sensor in dry soil/air -> DRY_READING
// - put sensor in very wet soil/water -> WET_READING
const int DRY_READING = 760;
const int WET_READING = 420;

float filteredRaw = 0.0f;
bool filterReady = false;
unsigned long lastPrintMs = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  analogReadResolution(10);

  Modulino.begin();
  pixels.begin();

  Serial.println("Moisture bar ready!");
  Serial.println("Sensor pin: A0 | Open Serial Monitor at 9600 baud.");
}

void loop() {
  int raw = analogRead(SENSOR_PIN);

  if (!filterReady) {
    filteredRaw = raw;
    filterReady = true;
  }

  // Smooth tiny jumps so LEDs feel stable.
  filteredRaw = (filteredRaw * 0.85f) + (raw * 0.15f);

  int moisturePercent = map((int)filteredRaw, DRY_READING, WET_READING, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  int lit = map(moisturePercent, 0, 100, 0, LED_COUNT);
  lit = constrain(lit, 0, LED_COUNT);

  for (int i = 0; i < LED_COUNT; i++) {
    if (i < lit) {
      int red = map(i, 0, LED_COUNT - 1, 255, 0);
      int green = map(i, 0, LED_COUNT - 1, 0, 255);
      pixels.set(i, ModulinoColor(red, green, 0), 18);
    } else {
      pixels.set(i, ModulinoColor(0, 0, 0), 0);
    }
  }
  pixels.show();

  if (millis() - lastPrintMs > 350) {
    lastPrintMs = millis();
    Serial.print("Raw: ");
    Serial.print(raw);
    Serial.print("  Smooth: ");
    Serial.print((int)filteredRaw);
    Serial.print("  Moisture %: ");
    Serial.println(moisturePercent);
  }

  delay(40);
}

