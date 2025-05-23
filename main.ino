#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "webserver.h"
#include "defaultLED.h"

#define LED_NO false

const int led1GPIO = 2;  
const int led2GPIO = 0;  
const int led3GPIO = 12; 
const int buttonGPIO = 14; 

const char* ssid = "your ssid";
const char* password = "your password";

bool isButtonPressed = false;
bool isTimerRunning = false;
unsigned long buttonPressTime = 0;
unsigned long previousMillis = 0;
const long interval = 500; 
const long holdThreshold = 2000; 
const long timerDelay = 15000; 

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(led1GPIO, OUTPUT);
  pinMode(led2GPIO, OUTPUT);
  pinMode(led3GPIO, OUTPUT);
  pinMode(buttonGPIO, INPUT_PULLDOWN_16);

  digitalWrite(led1GPIO, LOW);
  digitalWrite(led2GPIO, LOW);
  digitalWrite(led3GPIO, LOW);

  initWiFi();
  initServer();
}

void handleButtonAndTimer() {
  unsigned long currentMillis = millis();
  bool buttonState = digitalRead(buttonGPIO);

  if (buttonState == HIGH && !isButtonPressed) {
    buttonPressTime = currentMillis;
    isButtonPressed = true;
  }

  if (isButtonPressed && (currentMillis - buttonPressTime >= holdThreshold) && !isTimerRunning) {
    isTimerRunning = true;
    previousMillis = currentMillis;
  }

  if (isTimerRunning && (currentMillis - previousMillis >= timerDelay)) {
    isTimerRunning = false;
    isButtonPressed = false;
    buttonPressTime = 0;
    previousMillis = millis();
  }
}

void loop() {
  handleButtonAndTimer();
  if (!isTimerRunning) {
    defaultLEDSwitching();
  }
}
