#include <Modulino.h>

ModulinoDistance distanceSensor;
const long SERIAL_BAUD = 9600;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  distanceSensor.begin();

  Serial.println("Distance sensor ready!");
  Serial.println("Open Serial Monitor at 9600 baud.");
}

void loop() {
  if (distanceSensor.available()) {
    int mm = distanceSensor.get();
    Serial.print("Distance (mm): ");
    Serial.println(mm);
  } else {
    Serial.println("No fresh distance yet...");
  }

  delay(250);
}
