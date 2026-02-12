#include <Modulino.h>

ModulinoKnob knob;
int lastValue = -9999;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  knob.begin();
  knob.set(0);

  Serial.println("Turn the knob. Press it to reset to 0.");
}

void loop() {
  int value = knob.get();
  if (value != lastValue) {
    lastValue = value;
    Serial.print("Knob value: ");
    Serial.println(value);
  }

  if (knob.isPressed()) {
    knob.set(0);
    Serial.println("Knob button pressed! Counter reset.");
    delay(200);
  }

  delay(25);
}

