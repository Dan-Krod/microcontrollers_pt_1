#include "wifi_setup.h"
#include "webserver.h"
#include "defaultLED.h"
#include "uart_handler.h"

void setup() {
  Serial.begin(28800, SERIAL_8E1);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  initWiFi();
  initServer();
}

void loop() {
  handleSerialCommands();
  handleButtonAndTimer();
  ws.cleanupClients();
  if (!isTimerRunning) {
    defaultLEDSwitching();
  }
}
