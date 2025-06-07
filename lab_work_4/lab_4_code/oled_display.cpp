#include "oled_display.h"
#include <WiFi.h>
#include <NTPClient.h>

extern NTPClient timeClient;
extern int photoCount;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initOLED() {
  Wire.begin(40, 39);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("OLED initialized");
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(10, 5);
  display.println("Time: " + timeClient.getFormattedTime().substring(0, 5));

  display.drawLine(0, 20, SCREEN_WIDTH, 20, WHITE);

  display.setCursor(10, 25);
  display.print("IP: ");
  display.println(WiFi.localIP());

  display.drawLine(0, 45, SCREEN_WIDTH, 45, WHITE);

  display.setCursor(10, 50);
  display.print("Photos Sent: ");
  display.println(photoCount);

  display.display();
}
