#include <Modulino.h>

ModulinoMovement movement;
const long SERIAL_BAUD = 9600;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  movement.begin();

  Serial.println("Movement sensor ready!");
  Serial.println("Open Serial Monitor at 9600 baud.");
}

void loop() {
  movement.update();

  float x = movement.getX();
  float y = movement.getY();
  float z = movement.getZ();

  Serial.print("X: ");
  Serial.print(x, 2);
  Serial.print("  Y: ");
  Serial.print(y, 2);
  Serial.print("  Z: ");
  Serial.println(z, 2);

  delay(200);
}
