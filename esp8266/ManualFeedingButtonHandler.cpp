#include <Arduino.h>
#include "ManualFeedingButtonHandler.h"
#include "WifiHandler.h"
#include "MqttHandler.h"
#include "ServoHandler.h"
#include "config.h"

unsigned long buttonPressTime = 0;
const int debounceDelay = 50;
bool buttonPressed = false;

void setupButton() {
  pinMode(manualFeedingButton, INPUT_PULLUP); // Butonun bağlı olduğu pini giriş olarak ayarla
}

void checkButton() {
  static unsigned long lastDebounceTime = 0;
  bool buttonState = digitalRead(manualFeedingButton) == LOW;

  if (buttonState && !buttonPressed) { // Butona basıldığında
    if (millis() - lastDebounceTime > debounceDelay) {
      buttonPressed = true;
      buttonPressTime = millis();
    }
  }

  if (buttonPressed && millis() - buttonPressTime >= checkInterval * 1000) { // Yeterli süre basıldı ise besleme yap
      feed(1); // feedPet metodunu çağır (1 öğünlük)
      buttonPressTime = millis(); // Zamanlayıcıyı sıfırla
      
      if (client.connected()) {
        String message = "Düğme ile manuel besleme yapıldı.";
        client.publish(feedback_channel, message.c_str());
      } else {
        Serial.println("MQTT bağlantısı yok, manuel besleme yapıldı mesajı gönderilemedi.");
      }
    }
  
  if (!buttonState) {
    buttonPressed = false; // Buton bırakıldıysa sıfırla
  }
}
