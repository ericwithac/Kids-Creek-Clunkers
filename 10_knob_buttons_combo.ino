#include <Modulino.h>

ModulinoKnob knob;
ModulinoButtons buttons;

bool lastPressed[3] = {false, false, false};

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 4000) {
  }

  Modulino.begin();
  knob.begin();
  buttons.begin();
  knob.set(0);

  Serial.println("Turn knob to choose lane 1/2/3. Press matching button!");
}

void loop() {
  buttons.update();

  int value = knob.get();
  int lane = abs(value) % 3;

  bool led0 = (lane == 0);
  bool led1 = (lane == 1);
  bool led2 = (lane == 2);
  buttons.setLeds(led0, led1, led2);

  bool nowPressed[3] = {
    buttons.isPressed(0),
    buttons.isPressed(1),
    buttons.isPressed(2)
  };

  for (int i = 0; i < 3; i++) {
    if (nowPressed[i] && !lastPressed[i]) {
      if (i == lane) {
        Serial.println("Nice! Correct button.");
      } else {
        Serial.println("Oops! Try the lit button.");
      }
    }
    lastPressed[i] = nowPressed[i];
  }

  delay(25);
}

