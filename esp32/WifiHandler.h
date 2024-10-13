#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <WiFi.h>

// Wifi noktası ayarları
extern const char* ssid;
extern const char* password;

void startWiFi();
void resetWiFi();
void checkWiFiConnection();

#endif