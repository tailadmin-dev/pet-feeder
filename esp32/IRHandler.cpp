#include <Arduino.h>
#include "IRHandler.h"
#include "WifiHandler.h"
#include "MqttHandler.h"
#include "LedHandler.h"
#include "config.h"

unsigned long irlastCheckTime = 0; // Son kontrol zamanı
unsigned long ircheckInterval = 3600000; // 1 saatlik zaman aralığı (milisaniye cinsinden)
bool foodEmpty = false; // Mama kabı durumu

void setupIR() {
  pinMode(irPin, INPUT);
}

void sendEmptyFeederMessage(){
  String response = "Mama bitti, lütfen doldurun";
  Serial.println(response.c_str());
  client.publish(feedback_channel, response.c_str());
}

void checkFood() {
  unsigned long currentTime = millis();
  
  // IR sensörünü oku
  int read = digitalRead(irPin);
  
  // Mama kabı boşsa ve durum değişmişse veya belirlenen aralıkta tekrar kontrol gerekiyorsa
  if ((read == HIGH && !foodEmpty) || (foodEmpty && (currentTime - irlastCheckTime >= ircheckInterval))) {
    foodEmpty = true;
    sendEmptyFeederMessage();
    irlastCheckTime = currentTime; // Zamanlayıcıyı sıfırla
    currentLedColor = "blue";
  } else if (read == LOW) {
    foodEmpty = false; // Mama kabı dolu, durumu sıfırla
    currentLedColor = "green";
  }
}
