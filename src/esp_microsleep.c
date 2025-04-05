/*
 * esp_microsleep.c
 *
 * Copyright (c) 2023 Michael Lutz. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Original repository: https://github.com/mickeyl/esp-microsleep
 *
 * This version is adapted for use with the Arduino IDE (ESP32 core).
 */

#include "esp_microsleep.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Delay the current task for the specified number of microseconds.
 *
 * Uses esp_timer_get_time() for sub-millisecond delays. For delays under
 * 1000 microseconds, this function waits until the target time is reached,
 * yielding with taskYIELD() to avoid CPU hogging. For delays over 1000 µs,
 * vTaskDelay() is used.
 *
 * This version retains the original logic but is reorganized to build
 * cleanly under the Arduino ESP32 core.
 */
void esp_microsleep(uint64_t us) {
  if (us < 1000) {
    uint64_t target = esp_timer_get_time() + us;
    while (esp_timer_get_time() < target) {
      taskYIELD(); // Yield to avoid WDT reset
    }
  } else {
    vTaskDelay(us / 1000 / portTICK_PERIOD_MS);
  }
}
