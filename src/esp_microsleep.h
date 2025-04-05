/*
 * esp_microsleep.h
 *
 * Copyright (c) 2023 Michael Lutz. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 * 
 * Original repository: https://github.com/mickeyl/esp-microsleep
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Delay the current task for a given number of microseconds.
 *
 * For delays shorter than one millisecond, uses esp_timer and taskYIELD()
 * to avoid busy waiting. For longer delays, falls back to vTaskDelay().
 *
 * This version is adapted for Arduino IDE and retains the original
 * license and functionality from the ESP-IDF component.
 *
 * @param us Duration in microseconds.
 */
void esp_microsleep(uint64_t us);

#ifdef __cplusplus
}
#endif
