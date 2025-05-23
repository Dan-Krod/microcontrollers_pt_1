#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#define BUTTON_PIN 14  
#define COMMAND_FAST 0106  
#define COMMAND_STOP 0123  

bool isButtonPressed = false;
bool commandInProgress = false;
bool uartSent = false;

void handleSerialCommands() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == COMMAND_STOP && !uartSent) {
      isButtonPressed = true;
      uartSent = false;
    } else if (command >= '0' && command <= '9') {
      isButtonPressed = true;
    }
  }
}

#endif
