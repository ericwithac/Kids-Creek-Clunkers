#include <Modulino.h>

ModulinoPixels pixels;
ModulinoButtons buttons;

const int SENSOR_PIN = A0;
const int LED_COUNT = 8;
const long SERIAL_BAUD = 9600;
const unsigned long CAL_COUNTDOWN_MS = 3000;
const unsigned long CAL_SAMPLE_MS = 1200;
const int MIN_CAL_SPAN = 30;  // Minimum raw difference between dry and wet.

enum CalProcess {
  CAL_IDLE,
  CAL_DRY_COUNTDOWN,
  CAL_WET_COUNTDOWN,
  CAL_DRY_SAMPLING,
  CAL_WET_SAMPLING
};

CalProcess calProcess = CAL_IDLE;
unsigned long calProcessStartMs = 0;
unsigned long lastCountdownPrintMs = 0;

int dryCal = 700;
int wetCal = 500;
long calSum = 0;
int calCount = 0;
bool lastPressed[3] = {false, false, false};

float filteredRaw = 0.0f;
bool filterReady = false;
unsigned long lastPrintMs = 0;

void resetAccumulator() {
  calSum = 0;
  calCount = 0;
}

void addSample(int raw) {
  calSum += raw;
  calCount++;
}

int getAverage(int fallback) {
  if (calCount <= 0) {
    return fallback;
  }
  return (int)(calSum / calCount);
}

void drawSingleColorBar(int lit, const ModulinoColor &color, int brightness) {
  for (int i = 0; i < LED_COUNT; i++) {
    if (i < lit) {
      pixels.set(i, color, brightness);
    } else {
      pixels.set(i, ModulinoColor(0, 0, 0), 0);
    }
  }
  pixels.show();
}

void normalizeCalibration() {
  // Capacitive sensor should be: dry raw higher, wet raw lower.
  if (dryCal < wetCal) {
    int t = dryCal;
    dryCal = wetCal;
    wetCal = t;
    Serial.println("Calibration reversed. Swapped dry/wet.");
  }

  if (dryCal - wetCal < MIN_CAL_SPAN) {
    wetCal = dryCal - MIN_CAL_SPAN;
    if (wetCal < 0) {
      wetCal = 0;
    }
    Serial.println("Calibration span was too small, adjusted span.");
  }
}

void startCountdown(bool isDry) {
  calProcess = isDry ? CAL_DRY_COUNTDOWN : CAL_WET_COUNTDOWN;
  calProcessStartMs = millis();
  lastCountdownPrintMs = 0;
  Serial.println(isDry ? "Dry calibration requested." : "Wet calibration requested.");
  Serial.println("You have 3 seconds...");
}

void beginSampling(bool isDry) {
  calProcess = isDry ? CAL_DRY_SAMPLING : CAL_WET_SAMPLING;
  calProcessStartMs = millis();
  resetAccumulator();
  Serial.println(isDry ? "Sampling DRY now..." : "Sampling WET now...");
}

void finishSampling(bool isDry, int fallbackRaw) {
  int avg = getAverage(fallbackRaw);
  if (isDry) {
    dryCal = avg;
    Serial.print("Saved DRY calibration: ");
    Serial.println(dryCal);
  } else {
    wetCal = avg;
    Serial.print("Saved WET calibration: ");
    Serial.println(wetCal);
  }

  normalizeCalibration();
  filterReady = false;
  calProcess = CAL_IDLE;

  Serial.print("Using DryCal=");
  Serial.print(dryCal);
  Serial.print(" WetCal=");
  Serial.println(wetCal);
}

void updateButtonLeds() {
  if (calProcess == CAL_IDLE) {
    // Button 1 and 2 are active calibration buttons.
    buttons.setLeds(true, true, false);
    return;
  }

  bool blink = ((millis() / 180) % 2) == 0;
  if (calProcess == CAL_DRY_COUNTDOWN || calProcess == CAL_DRY_SAMPLING) {
    buttons.setLeds(blink, false, false);
    return;
  }

  if (calProcess == CAL_WET_COUNTDOWN || calProcess == CAL_WET_SAMPLING) {
    buttons.setLeds(false, blink, false);
    return;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial && millis() < 4000) {
  }

  analogReadResolution(10);

  Modulino.begin();
  pixels.begin();
  buttons.begin();
  buttons.setLeds(true, true, false);

  Serial.println("Moisture bar ready!");
  Serial.println("Sensor pin: A0 | Open Serial Monitor at 9600 baud.");
  Serial.println("Button 1 = calibrate DRY (3-second countdown).");
  Serial.println("Button 2 = calibrate WET (3-second countdown).");
  Serial.println("Button 3 = print current calibration.");
  Serial.print("Start DryCal=");
  Serial.print(dryCal);
  Serial.print(" WetCal=");
  Serial.println(wetCal);
}

void loop() {
  int raw = analogRead(SENSOR_PIN);
  buttons.update();

  bool nowPressed[3] = {
    buttons.isPressed(0),
    buttons.isPressed(1),
    buttons.isPressed(2)
  };

  if (nowPressed[0] && !lastPressed[0]) {
    startCountdown(true);
  }
  if (nowPressed[1] && !lastPressed[1]) {
    startCountdown(false);
  }
  if (nowPressed[2] && !lastPressed[2]) {
    Serial.print("Current DryCal=");
    Serial.print(dryCal);
    Serial.print(" WetCal=");
    Serial.println(wetCal);
  }
  lastPressed[0] = nowPressed[0];
  lastPressed[1] = nowPressed[1];
  lastPressed[2] = nowPressed[2];

  updateButtonLeds();

  if (calProcess == CAL_DRY_COUNTDOWN || calProcess == CAL_WET_COUNTDOWN) {
    unsigned long elapsed = millis() - calProcessStartMs;
    int remainingSec = (int)((CAL_COUNTDOWN_MS > elapsed ? (CAL_COUNTDOWN_MS - elapsed) : 0) / 1000) + 1;

    int progress = map((int)elapsed, 0, (int)CAL_COUNTDOWN_MS, 0, LED_COUNT);
    progress = constrain(progress, 0, LED_COUNT);
    if (calProcess == CAL_DRY_COUNTDOWN) {
      drawSingleColorBar(progress, ModulinoColor(255, 40, 0), 16);
    } else {
      drawSingleColorBar(progress, ModulinoColor(0, 110, 255), 16);
    }

    if (millis() - lastCountdownPrintMs > 900) {
      lastCountdownPrintMs = millis();
      Serial.print("Calibrating in ");
      Serial.print(remainingSec);
      Serial.println("...");
    }

    if (elapsed >= CAL_COUNTDOWN_MS) {
      beginSampling(calProcess == CAL_DRY_COUNTDOWN);
    }
    delay(20);
    return;
  }

  if (calProcess == CAL_DRY_SAMPLING || calProcess == CAL_WET_SAMPLING) {
    addSample(raw);
    unsigned long elapsed = millis() - calProcessStartMs;
    int progress = map((int)elapsed, 0, (int)CAL_SAMPLE_MS, 0, LED_COUNT);
    progress = constrain(progress, 0, LED_COUNT);
    if (calProcess == CAL_DRY_SAMPLING) {
      drawSingleColorBar(progress, ModulinoColor(255, 20, 0), 20);
    } else {
      drawSingleColorBar(progress, ModulinoColor(0, 170, 255), 20);
    }

    if (elapsed >= CAL_SAMPLE_MS) {
      finishSampling(calProcess == CAL_DRY_SAMPLING, raw);
    }
    delay(20);
    return;
  }

  if (!filterReady) {
    filteredRaw = raw;
    filterReady = true;
  }

  // Smooth tiny jumps so LEDs feel stable.
  filteredRaw = (filteredRaw * 0.86f) + (raw * 0.14f);

  int smoothInt = (int)filteredRaw;
  int span = max(MIN_CAL_SPAN, dryCal - wetCal);

  int moisturePercent = (int)((float)(dryCal - smoothInt) * 100.0f / (float)span);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Keep at least 1 red LED on, even when fully dry.
  int lit = 1 + map(moisturePercent, 0, 100, 0, LED_COUNT - 1);
  lit = constrain(lit, 1, LED_COUNT);

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
    Serial.print("  DryCal: ");
    Serial.print(dryCal);
    Serial.print("  WetCal: ");
    Serial.print(wetCal);
    Serial.print("  Moisture %: ");
    Serial.print(moisturePercent);
    Serial.print("  Lit LEDs: ");
    Serial.println(lit);
  }

  delay(40);
}
