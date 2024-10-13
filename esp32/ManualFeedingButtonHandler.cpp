#include <Arduino.h>
#include "ManualFeedingButtonHandler.h"
#include "ServoHandler.h"
#include "config.h"

unsigned long buttonPressTime = 0;
bool buttonPressed = false;

void setupButton() {
  pinMode(manualFeedingButton, INPUT_PULLUP); // Butonun bağlı olduğu pini giriş olarak ayarla
}

void checkButton() {
  if (digitalRead(manualFeedingButton) == LOW) { // Butona basıldığında
    if (!buttonPressed) {
      buttonPressed = true;
      buttonPressTime = millis();
    }

    if (millis() - buttonPressTime >= checkInterval * 1000) {
      feed(1); // feedPet metodunu çağır (1 öğünlük)
      buttonPressTime = millis(); // Zamanlayıcıyı sıfırla
    }
  } else {
    buttonPressed = false;
  }
}
