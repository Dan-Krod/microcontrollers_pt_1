#ifndef TIME_SYNC_H
#define TIME_SYNC_H

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000); // Київ UTC+3

void setupTime() {
  timeClient.begin();
}

void updateTime() {
  timeClient.update();
  String now = timeClient.getFormattedTime().substring(0, 5);
  Serial.println("🕒 Поточний час: [" + now + "]");
}

#endif
