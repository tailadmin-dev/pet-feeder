#include "config.h"

// WiFiHandler
const char* ssid = "WifiAdi"; // Wifi adı
const char* password = "Sifre"; //Wifi şifresi

//MqttHandler
const char* mqtt_server = "MqttServerIpAdresi"; // Mqtt sunucusu adresi
const int mqtt_port = 1883; // Mqtt sunucusunun portu
const unsigned long pingInterval = 300000;  // Ping periyodu
const char* feed_channel = "pet_feeder/feed"; // Besleme kanalı
const char* ping_channel = "pet_feeder/ping"; // Ping kanalı
const char* feedback_channel = "pet_feeder/feedback"; // Geri bildirim kanalı

//AutoFeedHandler
const int feeding_period = 5; // Besleme sıklığı
const int default_periodical_feeding_amount = 1; // Varsayılan öğün miktarı
const int GTMOffset = 3; // Zaman dilimi

//ManualFeedingButtonHandler
int manualFeedingButton = 19; // Manuel besleme düğmesi pini
unsigned long checkInterval = 3; // Besleme için düğmeye basılacak süre

//ServoHandler
int servoPin = 18; // Servo motor pini

//IRHandler
const int irPin = 23; // IR sensör pini

//LedHandler
const int ledRedPin = 27; // Kırmızı LED pini
const int ledGreenPin = 12; // Yeşil LED pini
const int ledBluePin = 13; // Mavi LED pini

