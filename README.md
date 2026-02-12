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

## Local compile testing in this workspace

This environment cannot download the real Modulino library from the internet,
so compile testing here uses a tiny local mock library at:

- `_mock_modulino_library`

That verifies sketch syntax and structure, but not real hardware behavior.
