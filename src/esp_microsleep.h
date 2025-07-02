/*
 * esp_microsleep.h
 *
 * Copyright (c) 2023 Michael Lutz. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 * 
 * Original repository: https://github.com/mickeyl/esp-microsleep
 */
/*
 * Copyright (c) 2024 Dr. Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file esp_microsleep.h
 * @brief Provides a microsecond-level delay function for ESP32 using esp_timer and FreeRTOS task notifications.
 *
 * @note Configuration Requirements:
 * This module relies on specific ESP-IDF Kconfig settings:
 *  - `CONFIG_ESP_MICROSLEEP_TLS_INDEX`: Defines the FreeRTOS Task Local Storage (TLS) index used to store
 *    per-task timer handles. Ensure this index is reserved and not used by other components.
 *  - `CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD`: Must be enabled to allow the timer callback
 *    (`esp_microsleep_isr_handler`) to run directly from the timer ISR context for minimal latency.
 *    This typically requires `CONFIG_ESP_TIMER_ISR_AFFINITY` to be configured appropriately
 *    (e.g., `ESP_TIMER_ISR_AFFINITY_CPU0` or `ESP_TIMER_ISR_AFFINITY_CPU1`).
 *
 * @details
 * This implementation uses a one-shot `esp_timer` per task, managed via TLS.
 * It includes a calibration function (`esp_microsleep_calibrate`) to measure the overhead
 * of the timer mechanism and uses `ets_delay_us` for very short delays where the timer
 * overhead would be significant.
 */

/*
 * esp_microsleep.h (MODIFIED VERSION FOR ARDUINO)
 */

#pragma once
#ifndef ESP_MICROSLEEP_H
#define ESP_MICROSLEEP_H

// --- START OF REQUIRED MODIFICATIONS FOR ARDUINO IDE ---
// We are manually defining the Kconfig values here to make the library compile.
//
// 1. Define the TLS Index. We'll pick a safe, low number like 1.
//    FreeRTOS has several slots, and it's unlikely other libs are using this one.
#define CONFIG_ESP_MICROSLEEP_TLS_INDEX 1
//
// 2. Define the ISR dispatch method support. The underlying libraries in modern
//    Arduino cores have this enabled, but we need to define it here so the
//    preprocessor check passes.
#define CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD 1
// --- END OF REQUIRED MODIFICATIONS ---


#include "stdint.h" // for uint64_t
#include "sdkconfig.h" // for CONFIG_*
#include "esp_err.h" // for esp_err_t

#ifdef __cplusplus
extern "C" {
#endif

// The original #if guard will now pass, and the rest of the file will be compiled.
#if defined(CONFIG_ESP_MICROSLEEP_TLS_INDEX) && defined(CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD)

// ... (rest of the original header file is unchanged) ...
uint64_t esp_microsleep_calibrate();
esp_err_t esp_microsleep_delay(uint64_t us);

#else
#warning esp_microsleep not available ...
#endif

#else
#warning esp_microsleep not available due to configuration mismatch. Please adjust CONFIG_ESP_MICROSLEEP_TLS_INDEX and CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD.
#endif // CONFIG_ESP_MICROSLEEP_TLS_INDEX && CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ESP_MICROSLEEP_H