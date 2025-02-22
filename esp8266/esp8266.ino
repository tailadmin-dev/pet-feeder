#include "WifiHandler.h"
#include "MqttHandler.h"
#include "ManualFeedingButtonHandler.h"
#include "ServoHandler.h"
#include "IRHandler.h"
#include "LedHandler.h"
#include "AutoFeedHandler.h"
#include <FS.h>

void setup() {
  ESP.wdtDisable();
  // Return serial port
  Serial.begin(115200);
  delay(500);
  ESP.wdtEnable(10000);
  
  Serial.println("\nESP8266 Başlatılıyor...");
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS başlatılamadı! Lütfen dosya sistemini kontrol edin.");
  } else {
    Serial.println("SPIFFS başarıyla başlatıldı.");
  }
  
  // Leds
  setupLeds();
  checkLedColor();

  //IR
  setupIR();

  // Wifi
  startWiFi();  // WiFi bağlantısını başlat

  // Mqtt
  setupMQTT();  // MQTT bağlantısını başlat

  // Manual Feeding Button
  setupButton();

  // Auto feed
  setupAutoFeed();
}

void loop() {
  checkLedColor();
  checkWiFiConnection();  // WiFi bağlantısını kontrol et
  checkMQTTConnection();  // MQTT bağlantısını kontrol et
  checkButton();          // Manel besleme düğmesini kontrol et
  checkFood();            // Mama kabı doluluğunu kontrol et
  checkFeeding();        // Besleme saatini kontrol et
}
