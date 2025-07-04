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
#include "esp_microsleep.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"

#if defined(CONFIG_ESP_MICROSLEEP_TLS_INDEX) && defined(CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD)

static uint64_t esp_microsleep_compensation = 0;

static void esp_microsleep_timer_delete_callback(int index, void *pvHandle) {
    if (pvHandle) {
        esp_timer_handle_t timer = (esp_timer_handle_t)pvHandle;
        esp_timer_delete(timer);
    }
}

// NOTE: We no longer need IRAM_ATTR here since it's not a direct ISR handler,
// but leaving it is harmless.
static void IRAM_ATTR esp_microsleep_isr_handler(void* arg) {
    TaskHandle_t task = (TaskHandle_t)(arg);
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // This function is safe to call from the high-priority timer task context.
    vTaskNotifyGiveFromISR(task, &higherPriorityTaskWoken);

    // We no longer call esp_timer_isr_dispatch_need_yield()
    // The timer task handles yielding if necessary.
    if (higherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

uint64_t esp_microsleep_calibrate() {
    // Calibration logic remains the same
    const int calibration_loops = 10;
    const uint64_t calibration_usec = 100;
    uint64_t compensation = 0;

    esp_microsleep_delay(0);
    for (int i = 0; i < calibration_loops; i++) {
        uint64_t start = esp_timer_get_time();
        esp_microsleep_delay(calibration_usec);
        uint64_t diff = esp_timer_get_time() - start - calibration_usec;
        compensation += diff;
    }
    esp_microsleep_compensation = compensation / calibration_loops;
    return esp_microsleep_compensation;
}

esp_err_t esp_microsleep_delay(uint64_t us) {
    esp_timer_handle_t timer = (esp_timer_handle_t) pvTaskGetThreadLocalStoragePointer(NULL, CONFIG_ESP_MICROSLEEP_TLS_INDEX);
    if (!timer) {
        // --- THE ONLY CHANGE IS HERE ---
        const esp_timer_create_args_t oneshot_timer_args = {
            .callback = &esp_microsleep_isr_handler,
            .arg = (void*) xTaskGetCurrentTaskHandle(),
            .dispatch_method = ESP_TIMER_TASK, // Use TASK dispatch instead of ISR
            .name = "microsleep_timer"
        };
        // --- END OF CHANGE ---

        esp_err_t err = esp_timer_create(&oneshot_timer_args, &timer);
        if (err != ESP_OK) {
            return err;
        }
        vTaskSetThreadLocalStoragePointerAndDelCallback(NULL,
                                                        CONFIG_ESP_MICROSLEEP_TLS_INDEX,
                                                        (void*) timer,
                                                        esp_microsleep_timer_delete_callback);
    }

    if (us == 0) { return ESP_OK; }

    if (us <= esp_microsleep_compensation) {
        ets_delay_us(us);
        return ESP_OK;
    }

    esp_err_t err = esp_timer_start_once(timer, us - esp_microsleep_compensation);
    if (err != ESP_OK) {
        return err;
    }
    
    // This correctly waits for the notification sent by our callback
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    return ESP_OK;
}

#endif
