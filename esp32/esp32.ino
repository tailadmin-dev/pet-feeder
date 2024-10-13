#include "WifiHandler.h"
#include "MqttHandler.h"
#include "ManualFeedingButtonHandler.h"
#include "ServoHandler.h"
#include "IRHandler.h"
#include "LedHandler.h"
#include "AutoFeedHandler.h"

void setup() {
  Serial.begin(115200);
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
