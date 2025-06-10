#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time_sync.h"
#include "firebase_setup.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
float oled_dist = 0.0;

void setupOLED() {
  Wire.begin(40, 39);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void updateDisplay(int mode) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  switch (mode) {
    case 0:
      display.setCursor(5, 5);
      display.println("Time: " + timeClient.getFormattedTime());
      display.setCursor(5, 20);
      display.println("Planned: " + scheduledTime);
      break;

    case 1:
      display.setCursor(5, 5);
      display.println("Distance: " + String(oled_dist) + " sm");

      float lastWeight = 0.0;
      if (Firebase.getFloat(firebaseData, "/weight_sensor/weight/last_measurement")) {
        lastWeight = firebaseData.to<float>();
      }

      display.setCursor(5, 20);
      display.println("Weight: " + String(lastWeight) + " g");
      break;
  }

  display.display();
}

#endif
