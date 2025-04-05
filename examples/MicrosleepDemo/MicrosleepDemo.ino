#include <esp_microsleep.h>

void setup() {
  Serial.begin(115200);
  Serial.println("esp-microsleep demo started.");
}

void loop() {
  Serial.println("Sleeping for 500 microseconds...");
  esp_microsleep(500);

  Serial.println("Sleeping for 2 milliseconds...");
  esp_microsleep(2000);

  delay(1000); // Just to pace output for readability
}
