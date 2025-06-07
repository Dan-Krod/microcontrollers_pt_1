#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"

extern const int buttonGPIO;
extern bool isButtonPressed;
extern bool isTimerRunning;
extern unsigned long buttonPressTime;
extern unsigned long previousMillis;
extern const long holdThreshold;
extern const long timerDelay;

extern const char* ssid;
extern const char* password;
extern AsyncWebServer server;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());
}

void initServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    buttonPressTime = millis();
    isButtonPressed = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    isButtonPressed = false;
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound);
  server.begin();
}

#endif
