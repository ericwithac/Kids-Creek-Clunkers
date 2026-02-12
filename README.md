# Modulino Kids Examples (UNO R4 WiFi)

This folder has simple Arduino sketches for the Modulino units:

- Thermo
- Distance
- Movement
- Pixels
- Buzzer
- Knob
- Buttons

It also includes easy combo examples:

- Knob + Pixels
- Buttons + Buzzer

## Before you upload to your board

1. In Arduino IDE, install:
- **Arduino UNO R4 Boards**
- **Modulino** library

2. Select board:
- **Arduino UNO R4 WiFi**

3. Open one `.ino` file in this folder and upload.

4. For sensor text output (Thermo/Distance/Movement):
- Open **Serial Monitor** in Arduino IDE.
- Set baud rate to **9600**.
- Set line ending to **No line ending** (or any option, it does not matter for these read-only sketches).

## Sketch list

- `01_thermo_simple.ino`
- `02_distance_simple.ino`
- `03_movement_simple.ino`
- `04_pixels_rainbow.ino`
- `05_buzzer_song.ino`
- `06_knob_counter.ino`
- `07_buttons_reactions.ino`
- `08_knob_pixels_combo.ino`
- `09_buttons_buzzer_combo.ino`
- `10_knob_buttons_combo.ino`
- `11_distance_buzzer_theremin.ino`
- `12_movement_pixels_tilt.ino`
- `13_thermo_pixels_heatbar.ino`
- `14_distance_buttons_buzzer_theremin_scale/14_distance_buttons_buzzer_theremin_scale.ino`
- `15_moisture_pixels_bar.ino`

## Moisture Sensor (Script 15)

Wire capacitive moisture sensor v1.2 like this:

- `AOUT` -> `A0`
- `VCC` -> `5V`
- `GND` -> `GND`

Then open:

- `15_moisture_pixels_bar.ino`
