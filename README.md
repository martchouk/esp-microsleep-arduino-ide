# esp-microsleep (Arduino-compatible version)

This is an **Arduino IDE–compatible version** of [Michael Lutz’s original `esp-microsleep`](https://github.com/mickeyl/esp-microsleep) project. It allows **ESP32-based Arduino sketches** to perform sub-millisecond task delays without blocking the CPU or triggering watchdog resets.

## Features

- Microsecond-precision delays for ESP32 FreeRTOS tasks
- Works seamlessly in Arduino IDE 2.x+
- Non-blocking: uses `taskYIELD()` to avoid CPU hogging
- Automatically uses `vTaskDelay()` for delays ≥ 1 ms
- Lightweight and hardware-backed (uses `esp_timer_get_time()`)

## Installation

### Option 1: Add via ZIP
1. Download this repo as a ZIP file
2. Open **Arduino IDE**
3. Go to **Sketch → Include Library → Add .ZIP Library...**
4. Select the downloaded ZIP

### Option 2: Install via GitHub (Arduino IDE 2.x)
1. Open Arduino IDE
2. Go to **Sketch -> Include Library -> Add Library -> Install from GitHub**
3. Enter your fork's URL: https://github.com/martchouk/esp-microsleep-arduino-ide

## Usage Example

```cpp
#include <esp_microsleep.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Sleeping for 500 microseconds...");
  esp_microsleep(500);  // precise sleep
}
```

## Project Structure

```
esp-microsleep-arduino/
├── LICENSE
├── README.md
├── library.properties
├── src/
│   ├── esp_microsleep.c
│   └── esp_microsleep.h
└── examples/
    └── MicrosleepDemo/
        └── MicrosleepDemo.ino
```

## License and Attribution

This library is based on [esp-microsleep](https://github.com/mickeyl/esp-microsleep)  
by Dr. Michael 'Mickey' Lauer (© 2024), and is licensed under the MIT License.

This fork adapts the original ESP-IDF component to be compatible with the Arduino IDE.  

This project includes the original LICENSE file as required by the license terms.

