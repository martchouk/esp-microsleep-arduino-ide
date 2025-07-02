/*
 * esp_microsleep.h (MODIFIED for Arduino IDE)
 *
 * This version uses the original library's structure with the necessary
 * preprocessor definitions added at the top to ensure compilation under
 * the Arduino environment.
 *
 * Copyright (c) 2023 Michael Lutz. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Original repository: https://github.com/mickeyl/esp-microsleep
 */

#pragma once
#ifndef ESP_MICROSLEEP_H
#define ESP_MICROSLEEP_H

// --- START OF REQUIRED MODIFICATIONS FOR ARDUINO IDE ---
// manually define the Kconfig values here to make the library compile.
#define CONFIG_ESP_MICROSLEEP_TLS_INDEX 1
#define CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD 1
// --- END OF REQUIRED MODIFICATIONS ---


#include "stdint.h"
#include "sdkconfig.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// This is the single, correct conditional block from the original library.
// Our defines at the top ensure this #if statement will pass.
#if defined(CONFIG_ESP_MICROSLEEP_TLS_INDEX) && defined(CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD)

/**
 * @brief Calibrate the microsleep compensation value.
 */
uint64_t esp_microsleep_calibrate();

/**
 * @brief Delays the calling task for a specified number of microseconds.
 */
esp_err_t esp_microsleep_delay(uint64_t us);

#else

// This #warning will now only trigger if our defines are removed.
#warning esp_microsleep not available due to configuration mismatch.

#endif // This closes the #if defined(...) block


#ifdef __cplusplus
}
#endif // This closes the extern "C" block

#endif // This closes the #ifndef ESP_MICROSLEEP_H include guard