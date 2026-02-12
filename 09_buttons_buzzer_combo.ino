#include <Modulino.h>

ModulinoButtons buttons;
ModulinoBuzzer buzzer;

bool lastPressed[3] = {false, false, false};
const int buttonNotes[3] = {262, 330, 392}; // Do, Mi, Sol

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  buttons.begin();
  buzzer.begin();
  buttons.setLeds(false, false, false);

  Serial.println("Press buttons to play notes!");
}

void loop() {
  buttons.update();

  bool nowPressed[3] = {
    buttons.isPressed(0),
    buttons.isPressed(1),
    buttons.isPressed(2)
  };

  for (int i = 0; i < 3; i++) {
    if (nowPressed[i] && !lastPressed[i]) {
      buzzer.tone(buttonNotes[i], 180);
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.println(" note!");
    }
    lastPressed[i] = nowPressed[i];
  }

  buttons.setLeds(nowPressed[0], nowPressed[1], nowPressed[2]);
  delay(20);
}

