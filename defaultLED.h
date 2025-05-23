#ifndef DEFAULT_LED_H
#define DEFAULT_LED_H

#define LED1_PIN 2  
#define LED2_PIN 0  
#define LED3_PIN 12  

unsigned long previousMillis = 0;
const long interval = 700;

void defaultLEDSwitching() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    static int ledCounter = 0;
    switch (ledCounter) {
      case 0:
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED3_PIN, LOW);
        break;
      case 1:
        digitalWrite(LED2_PIN, HIGH);
        digitalWrite(LED1_PIN, LOW);
        break;
      case 2:
        digitalWrite(LED3_PIN, HIGH);
        digitalWrite(LED2_PIN, LOW);
        ledCounter = -1;
        break;
    }
    ledCounter++;
    sendLedStatus();
  }
}

#endif
