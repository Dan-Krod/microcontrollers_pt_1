#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

const char* ssid = "your ssid";
const char* password = "your password";

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi підключено!");
}

#endif
