#include <Modulino.h>

ModulinoThermo thermo;
const long SERIAL_BAUD = 9600;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
    // Wait for Serial Monitor on startup (optional).
  }

  Modulino.begin();
  thermo.begin();

  Serial.println("Thermo ready!");
  Serial.println("Open Serial Monitor at 9600 baud.");
}

void loop() {
  float tempC = thermo.getTemperature();
  float humidity = thermo.getHumidity();

  Serial.print("Temp C: ");
  Serial.print(tempC, 1);
  Serial.print(" | Humidity %: ");
  Serial.println(humidity, 1);

  delay(1000);
}
