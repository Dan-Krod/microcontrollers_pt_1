#ifndef DEFAULT_LED_H
#define DEFAULT_LED_H

#include <Arduino.h>

extern const int led1GPIO;
extern const int led2GPIO;
extern const int led3GPIO;
extern unsigned long previousMillis;
extern const long interval;

void defaultLEDSwitching() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    static int ledCounter = 0;
    switch (ledCounter) {
      case 0:
        digitalWrite(led1GPIO, HIGH);
        digitalWrite(led3GPIO, LOW);
        break;
      case 1:
        digitalWrite(led2GPIO, HIGH);
        digitalWrite(led1GPIO, LOW);
        break;
      case 2:
        digitalWrite(led3GPIO, HIGH);
        digitalWrite(led2GPIO, LOW);
        ledCounter = -1;
        break;
    }
    ledCounter++;
  }
}

#endif
