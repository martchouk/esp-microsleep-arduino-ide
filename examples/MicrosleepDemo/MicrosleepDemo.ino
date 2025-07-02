#include <Arduino.h>

// 1. Include the header for the high-precision delay library
#include "esp_microsleep.h"

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for Serial Monitor to connect
  Serial.println("\n--- High-Precision Delay Demo ---");

  // 2. (Optional but Recommended) Calibrate the timer once at startup.
  // This measures the system's timer overhead to make the delays even more accurate.
  uint64_t compensation = esp_microsleep_calibrate();
  Serial.printf("esp_microsleep timer calibrated. Overhead compensation: %llu us\n\n", compensation);
}

void loop() {
  uint64_t start_time, end_time, duration;

  // --- Example 1: A short, sub-millisecond delay ---
  Serial.println("Testing a 500 microsecond delay...");
  start_time = esp_timer_get_time();
  
  // 3. Use the new, correct function name: esp_microsleep_delay()
  esp_microsleep_delay(500); 
  
  end_time = esp_timer_get_time();
  duration = end_time - start_time;
  Serial.printf("  > Actual elapsed time: %llu us\n\n", duration);


  // --- Example 2: A longer delay ---
  // Note: For delays over 1000 us, the library internally uses vTaskDelay,
  // so this is functionally similar but keeps your code consistent.
  Serial.println("Testing a 2000 microsecond (2ms) delay...");
  start_time = esp_timer_get_time();
  
  esp_microsleep_delay(2000);
  
  end_time = esp_timer_get_time();
  duration = end_time - start_time;
  Serial.printf("  > Actual elapsed time: %llu us\n\n", duration);


  // Standard Arduino delay to pace the output and make it readable.
  delay(2000); 
}
