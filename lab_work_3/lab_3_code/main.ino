#include "wifi_setup.h"
#include "oled_display.h"
#include "firebase_setup.h"
#include "time_sync.h"
#include "ultrasonic_sensor.h"
#include "weight_sensor.h"

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupOLED();
  setupFirebase();
  setupTime();
  setupUltrasonic();
  setupWeightSensor();
}

void loop() {
  updateTime();
  fetchScheduledTime();
  checkObstacle();
  checkWeightMeasurement();

  static unsigned long lastDisplayUpdate = 0;
  static int displayMode = 0;
  if (millis() - lastDisplayUpdate > 5000) {
    lastDisplayUpdate = millis();
    displayMode = (displayMode + 1) % 2;
    updateDisplay(displayMode);
  }

  delay(200);
}
