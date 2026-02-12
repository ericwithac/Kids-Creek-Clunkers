#include <Modulino.h>

ModulinoButtons buttons;

bool lastPressed[3] = {false, false, false};

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  buttons.begin();
  buttons.setLeds(false, false, false);

  Serial.println("Press any button!");
}

void loop() {
  buttons.update();

  bool b0 = buttons.isPressed(0);
  bool b1 = buttons.isPressed(1);
  bool b2 = buttons.isPressed(2);
  bool nowPressed[3] = {b0, b1, b2};

  for (int i = 0; i < 3; i++) {
    if (nowPressed[i] && !lastPressed[i]) {
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.println(" pressed!");
    }
    lastPressed[i] = nowPressed[i];
  }

  buttons.setLeds(b0, b1, b2);
  delay(25);
}

