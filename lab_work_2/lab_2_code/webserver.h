#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "webpage.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("Client connected");
        sendLedStatus();
    }
}

void sendLedStatus() {
    DynamicJsonDocument doc(256);
    doc["led1"] = digitalRead(LED1_PIN) ? "ON" : "OFF";
    doc["led2"] = digitalRead(LED2_PIN) ? "ON" : "OFF";
    doc["led3"] = digitalRead(LED3_PIN) ? "ON" : "OFF";
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
}

void initServer() {
  ws.onEvent(wsEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    isButtonPressed = true;
    request->send(200, "text/plain", "ok");
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    isButtonPressed = false;
    request->send(200, "text/plain", "ok");
  });
  server.on("/increaseSpeed", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!commandInProgress) {
      commandInProgress = true;
      Serial.write(COMMAND_FAST);
      uartSent = true;
      commandInProgress = false;
    }
    request->send(200, "text/plain", "Increase speed command sent");
  });
  server.begin();
}

#endif
